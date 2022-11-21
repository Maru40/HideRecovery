/*!
@file ToBallRunTask.cpp
@brief ToBallRunTaskなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "ToBallRunTask.h"

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

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"
#include "Maruyama/Item/HideItemObject.h"

#include "Itabashi/OnlinePlayerSynchronizer.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Task {

				//--------------------------------------------------------------------------------------
				///	ボールへ向かうタスクのパラメータ
				//--------------------------------------------------------------------------------------

				ToBallRunTask_Parametor::ToBallRunTask_Parametor() :
					moveAstarParam(new basecross::Task::MoveAstar::Parametor()),
					targetSeekParam(new TaskListNode::TargetSeek::Parametor()),
					waitParam(new basecross::Task::Wait::Parametor(0.5f))
				{}

				ToBallRunTask_Parametor::~ToBallRunTask_Parametor() {
					delete(moveAstarParam);
					delete(targetSeekParam);
				}

				//--------------------------------------------------------------------------------------
				///	ボールへ向かうタスクの本体
				//--------------------------------------------------------------------------------------

				ToBallRunTask::ToBallRunTask(const std::shared_ptr<Enemy::EnemyBase>& owner):
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
					m_onlineSynchronizer = object->GetComponent<OnlinePlayerSynchronizer>();
				}

				void ToBallRunTask::OnStart() {
					CalculateTarget();	//ターゲットの計算

					SelectTask();		//タスクの選択
				}

				bool ToBallRunTask::OnUpdate() {
					m_taskList->UpdateTask();
					Rotation();
					CheckForceNextMoveArriveTask();

					return IsEnd();
				}

				void ToBallRunTask::OnExit() {
					m_taskList->ForceStop();
				}

				std::shared_ptr<GameObject> ToBallRunTask::CalculateTarget() {
					auto hideObjects = ShareClassesManager::GetInstance()->GetCastShareClasses<HideItemObject>();

					m_targetManager.lock()->SetTarget(hideObjects[0].lock());
					return hideObjects[0].lock();
				}

				void ToBallRunTask::CheckForceNextMoveArriveTask() {
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

				void ToBallRunTask::DefineTask() {
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

					//待機行動
					DefineWaitTask();
				}

				void ToBallRunTask::DefineWaitTask() {
					//開始イベント
					m_param.waitParam->start = [&]() {
						m_onlineSynchronizer.lock()->Aquisition();
					};

					//終了イベント
					m_param.waitParam->exit = [&]() {
						
					};

					//タスクの定義
					m_taskList->DefineTask(
						TaskEnum::GetBall,
						std::make_shared<basecross::Task::Wait>(m_param.waitParam)
					);
				}

				void ToBallRunTask::SelectTask() {
					TaskEnum tasks[] = {
						TaskEnum::MoveAstar,
						TaskEnum::MoveArrive,
						//TaskEnum::GetBall,
					};

					for (const auto& task : tasks) {
						m_taskList->AddTask(task);
					}
				}

				void ToBallRunTask::InitializeParametor() {
					constexpr float MoveSpeed = 8.5f;
					constexpr float NearTargetRange = 1.5f;

					//Astarで目標の近くまで移動するパラメータ
					m_param.moveAstarParam->movePositionsParam->moveParamPtr->speed = MoveSpeed;
					m_param.moveAstarParam->movePositionsParam->moveParamPtr->targetNearRange = 1.5f;
					m_param.moveAstarParam->movePositionsParam->moveParamPtr->moveType = basecross::Task::ToTargetMove_MoveType::SeekVelocity;

					//目的地の近くまで移動した場合の設定
					m_param.targetSeekParam->toTargetMoveParam->speed = MoveSpeed;
					m_param.targetSeekParam->toTargetMoveParam->moveType = basecross::Task::ToTargetMove::MoveType::SeekVelocity;
					m_param.targetSeekParam->toTargetMoveParam->targetNearRange = NearTargetRange;
				}

				void ToBallRunTask::Rotation() {
					auto rotationController = m_rotationController.lock();
					auto velocityManager = m_velocityManager.lock();
					if (!velocityManager || !rotationController) {
						return;
					}

					rotationController->SetDirection(velocityManager->GetVelocity());
				}

				bool ToBallRunTask::IsEnd() const { return m_taskList->IsEnd(); }

			}
		}
	}
}