/*!
@file RelifMember.cpp
@brief RelifMemberなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "RelifMember.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Utility/Timer/GameTimer.h"
#include "Maruyama/Utility/Component/TargetManager.h"
#include "VelocityManager.h"

#include "Maruyama/TaskList/TaskList.h"
#include "Maruyama/TaskList/CommonTasks/Task_MovePositions.h"
#include "Maruyama/TaskList/CommonTasks/MoveAstar.h"
#include "Maruyama/TaskList/CommonTasks/Task_ToTargetMove.h"
#include "Maruyama/TaskList/CommonTasks/TargetSeek.h"
#include "Maruyama/TaskList/CommonTasks/OpenBox.h"
#include "Maruyama/TaskList/CommonTasks/Task_Wait.h"
#include "Maruyama/TaskList/CommonTasks/Task_AroundEyeCheck.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Enemy/ImpactMap/FieldImpactMap.h"
#include "Maruyama/Enemy/AIDirector/EnemyAIDirector.h"

#include "Maruyama/Interface/I_FactionMember.h"
#include "Maruyama/Enemy/AIDirector/FactionCoordinator.h"
#include "Maruyama/Enemy/AIDirector/PatrolCoordinator.h"
#include "Maruyama/Enemy/AIDirector/PatrolCoordinator/HidePlacePatrol.h"

#include "Maruyama/Item/HideItem.h"
#include "Maruyama/StageObject/HidePlace.h"

#include "Maruyama/Enemy/Component/EyeSearchRange.h"
#include "Maruyama/Utility/Component/RotationController.h"
#include "Maruyama/Utility/UtilityObstacle.h"

#include "Maruyama/Player/Component/ItemBag.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Task {

				//--------------------------------------------------------------------------------------
				///	見方の周りの警戒タスクパラメータ
				//--------------------------------------------------------------------------------------

				RelifMember_Parametor::RelifMember_Parametor() :
					moveAstarParam(new basecross::Task::MoveAstar::Parametor()),
					targetSeekParam(new TaskListNode::TargetSeek::Parametor()),
					aroundEyeCheckParam(new basecross::Task::AroundEyeCheck_Parametor()),
					waitParam(new basecross::Task::Wait::Parametor(0.5f))
				{}

				RelifMember_Parametor::~RelifMember_Parametor() {
					delete(moveAstarParam);
					delete(targetSeekParam);
				}

				//--------------------------------------------------------------------------------------
				///	見方の周りの警戒タスク本体
				//--------------------------------------------------------------------------------------

				RelifMember::RelifMember(const std::shared_ptr<Enemy::EnemyBase>& owner):
					TaskBase(owner),
					m_param(Parametor()),
					m_taskList(new TaskList<TaskEnum>())
				{
					InitializeParametor();
					DefineTask();

					auto object = GetOwner()->GetGameObject();
					m_transform = object->GetComponent<Transform>();
					m_eyeRange = object->GetComponent<EyeSearchRange>();
					m_targetManager = object->GetComponent<TargetManager>();
					m_velocityManager = object->GetComponent<VelocityManager>();
					m_rotationController = object->GetComponent<RotationController>();
					m_factionMember = object->GetComponent<Enemy::I_FactionMember>();
				}

				void RelifMember::OnStart() {
					CalculateTarget();	//ターゲットの計算

					SelectTask();		//タスクの選択
				}

				bool RelifMember::OnUpdate() {
					m_taskList->UpdateTask();
					Rotation();
					CheckForceNextMoveArriveTask();

					return IsEnd();
				}

				void RelifMember::OnExit() {
					m_taskList->ForceStop();
				}

				std::shared_ptr<GameObject> RelifMember::CalculateTarget() {
					auto assignedFaction = GetOwner()->GetAssignedFaction();
					if (!assignedFaction) {
						return nullptr;
					}

					for (auto& weakMember : assignedFaction->GetMembers()) {
						auto member = weakMember.lock();
						if (member == GetOwner()) {	//自分自身なら処理を飛ばす。
							continue;
						}

						auto itemBag = member->GetGameObject()->GetComponent<ItemBag>(false);
						if (!itemBag) {	//アイテムを持っていないなら処理を飛ばす。
							continue;
						}

						auto hideItem = itemBag->GetHideItem();
						if (hideItem) {	//隠しアイテムを持っているなら、ターゲットにする。
							auto target = member->GetGameObject();
							m_targetManager.lock()->SetTarget(target);
							return target;
						}
					}

					return nullptr;
				}

				void RelifMember::CheckForceNextMoveArriveTask() {
					if (m_taskList->IsEnd() || !m_targetManager.lock()->HasTarget()) {
						return;
					}

					//Astar移動でなかったら処理をしない
					if (m_taskList->GetCurrentTaskType() != TaskEnum::MoveAstar) {
						return;
					}

					//ターゲットが障害物無の状態なら
					auto startPosition = m_transform.lock()->GetPosition();
					auto targetPosition = m_targetManager.lock()->GetTargetPosition();
					auto toTargetPosition = targetPosition - startPosition;

					//ターゲットの距離が視界距離より大きかったら、処理をしない。
					if (m_eyeRange.lock()->GetEyeLength() < toTargetPosition.length()) {
						return;
					}

					//Ray判定を取る。
					auto objects = GetOwner()->GetGameObject()->GetStage()->GetGameObjectVec();
					if (!maru::UtilityObstacle::IsRayObstacle(startPosition, targetPosition, objects)) {
						m_taskList->ForceNextTask();
					}
				}

				void RelifMember::DefineTask() {
					auto ownerObject = GetOwner()->GetGameObject();

					//Astar移動
					m_taskList->DefineTask(
						TaskEnum::MoveAstar,
						std::make_shared<basecross::Task::MoveAstar>(GetOwner(), m_param.moveAstarParam)
					);

					//目的地の近くについたら時の到着移動
					m_taskList->DefineTask(
						TaskEnum::MoveArrive,
						std::make_shared<TaskListNode::TargetSeek>(ownerObject, m_param.targetSeekParam)
					);

					//ターゲットの周りを警戒する。
					m_taskList->DefineTask(
						TaskEnum::AroundPtrol,
						std::make_shared<basecross::Task::AroundEyeCheck>(ownerObject, m_param.aroundEyeCheckParam)
					);

					//待機行動
					DefineWaitTask();
				}

				void RelifMember::DefineWaitTask() {
					//開始イベント
					m_param.waitParam->start = [&]() {

					};

					//終了イベント
					m_param.waitParam->exit = [&]() {

					};

					//タスクの定義
					m_taskList->DefineTask(
						TaskEnum::Wait,
						std::make_shared<basecross::Task::Wait>(m_param.waitParam)
					);
				}

				void RelifMember::SelectTask() {
					TaskEnum tasks[] = {
						TaskEnum::MoveAstar,
						TaskEnum::MoveArrive,
						//TaskEnum::AroundPtrol,
						TaskEnum::Wait
					};

					for (const auto& task : tasks) {
						m_taskList->AddTask(task);
					}
				}

				void RelifMember::InitializeParametor() {
					constexpr float MoveSpeed = 8.5f;
					constexpr float NearTargetRange = 3.5f;

					//Astarで目標の近くまで移動するパラメータ
					m_param.moveAstarParam->movePositionsParam->moveParamPtr->speed = MoveSpeed;
					m_param.moveAstarParam->movePositionsParam->moveParamPtr->targetNearRange = 1.5f;
					m_param.moveAstarParam->movePositionsParam->moveParamPtr->moveType = basecross::Task::ToTargetMove_MoveType::SeekVelocity;

					//目的地の近くまで移動した場合の設定
					m_param.targetSeekParam->toTargetMoveParam->speed = MoveSpeed;
					m_param.targetSeekParam->toTargetMoveParam->moveType = basecross::Task::ToTargetMove::MoveType::ArriveVelocity;
					m_param.targetSeekParam->toTargetMoveParam->targetNearRange = NearTargetRange;
				}

				void RelifMember::Rotation() {
					auto rotationController = m_rotationController.lock();
					auto velocityManager = m_velocityManager.lock();
					if (!velocityManager || !rotationController) {
						return;
					}

					rotationController->SetDirection(velocityManager->GetVelocity());
				}

				bool RelifMember::IsEnd() const { return m_taskList->IsEnd(); }

			}
		}
	}
}