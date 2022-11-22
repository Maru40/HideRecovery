/*!
@file BehaviorTask_Shot.cpp
@brief BehaviorTask_Shotなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "BehaviorTask_Shot.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Itabashi/OnlinePlayerSynchronizer.h"

#include "Maruyama/Player/Component/UseWeapon.h"
#include "Maruyama/Player/Component/WeponBase.h"
#include "Maruyama/Enemy/Component/EyeSearchRange.h"
#include "Maruyama/Utility/Component/TargetManager.h"
#include "VelocityManager.h"
#include "Maruyama/Utility/Component/RotationController.h"

#include "Maruyama/TaskList/TaskList.h"
#include "Maruyama/TaskList/CommonTasks/CrabWalk.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Task {

				//--------------------------------------------------------------------------------------
				///	撃つ攻撃処理パラメータ
				//--------------------------------------------------------------------------------------

				Shot_Parametor::Shot_Parametor():
					crabWalkParamPtr(new TaskListNode::CrabWalk_Parametor())
				{}

				Shot_Parametor::~Shot_Parametor()
				{
					delete(crabWalkParamPtr);
				}

				//--------------------------------------------------------------------------------------
				///	撃つ攻撃処理本体
				//--------------------------------------------------------------------------------------

				Shot::Shot(const std::shared_ptr<Enemy::EnemyBase>& owner) :
					TaskBase(owner),
					m_param(Parametor()),
					m_moveTaskList(new TaskList<MoveTaskEnum>())
				{
					auto object = owner->GetGameObject();

					m_useWeapon = object->GetComponent<UseWeapon>(false);
					m_targetManager = object->GetComponent<TargetManager>(false);
					m_eyeRange = object->GetComponent<EyeSearchRange>(false);
					m_velocityManager = object->GetComponent<VelocityManager>(false);
					m_rotationController = object->GetComponent<RotationController>(false);
					m_onlineSynchronizer = object->GetComponent<OnlinePlayerSynchronizer>(false);

					DefineMoveTask();
				}

				void Shot::OnStart() {
					m_onlineSynchronizer.lock()->Aim(true);
					//m_useWeapon.lock()->SetIsAim(true);

					SelectMoveTask();
				}

				bool Shot::OnUpdate() {
					MoveUpdate();
					RotationUpdate();
					ShotUpdate();

					return IsEnd();
				}

				void Shot::OnExit() {
					//m_useWeapon.lock()->SetIsAim(false);
					m_onlineSynchronizer.lock()->Aim(false);
					m_velocityManager.lock()->ResetAll();

					m_moveTaskList->ForceStop();	//タスクの終了
				}

				void Shot::MoveUpdate() {
					//左右のどちらかか、泊まって撃つ。
					m_moveTaskList->UpdateTask();

					if (m_moveTaskList->IsEnd()) {
						SelectMoveTask();
					}
				}

				void Shot::RotationUpdate() {
					auto targetManager = m_targetManager.lock();
					if (!targetManager || !targetManager->HasTarget()) {	//ターゲットが存在しないなら処理を省く
						return;
					}

					m_onlineSynchronizer.lock()->ChangeDefaultForward(targetManager->CalcuToTargetVec());
					//m_rotationController.lock()->SetDirection(targetManager->CalcuToTargetVec());
				}

				void Shot::ShotUpdate() {
					m_onlineSynchronizer.lock()->Shot();
				}

				void Shot::DefineMoveTask() {
					auto object = GetOwner()->GetGameObject();

					m_moveTaskList->DefineTask(MoveTaskEnum::CrabWalk, std::make_shared<TaskListNode::CrabWalk>(object, m_param.crabWalkParamPtr));
				}

				void Shot::SelectMoveTask() {
					MoveTaskEnum tasks[] = {
						MoveTaskEnum::CrabWalk,
					};

					for (auto& task : tasks) {
						m_moveTaskList->AddTask(task);
					}
				}

				bool Shot::IsEnd() const {
					return false;
				}

			}
		}
	}
}