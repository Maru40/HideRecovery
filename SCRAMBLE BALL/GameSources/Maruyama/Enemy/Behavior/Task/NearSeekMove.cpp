/*!
@file Task_SearchBall.cpp
@brief Task_SearchBallなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "NearSeekMove.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/TaskList/TaskList.h"
#include "Maruyama/TaskList/CommonTasks/TargetSeek.h"

#include "Maruyama/Utility/Component/RotationController.h"
#include "Maruyama/Utility/Component/TargetManager.h"

#include "Maruyama/TaskList/CommonTasks/Task_ToTargetMove.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Task {

				//--------------------------------------------------------------------------------------
				///	ターゲットに近づくパラメータ
				//--------------------------------------------------------------------------------------

				NearSeekMove_Parametor::NearSeekMove_Parametor():
					moveParamPtr(new TaskListNode::TargetSeek_Parametor())
				{}

				NearSeekMove_Parametor::~NearSeekMove_Parametor() {
					delete(moveParamPtr);
				}

				//--------------------------------------------------------------------------------------
				///	ターゲットに近づく本体
				//--------------------------------------------------------------------------------------

				NearSeekMove::NearSeekMove(const std::shared_ptr<Enemy::EnemyBase>& owner, const Parametor* paramPtr):
					TaskBase(owner),
					m_paramPtr(paramPtr),
					m_taskList(new TaskList<TaskEnum>())
				{
					auto object = owner->GetGameObject();

					m_targetManager = object->GetComponent<TargetManager>(false);
					m_rotationController = object->GetComponent<RotationController>(false);

					DefineTask();
				}

				void NearSeekMove::OnStart() {
					SelectTask();
				}

				bool NearSeekMove::OnUpdate() {
					m_taskList->UpdateTask();
					RotationUpdate();

					return IsEnd();
				}

				void NearSeekMove::OnExit() {
					m_taskList->ForceStop();
				}

				void NearSeekMove::SelectTask() {
					TaskEnum tasks[] = {
						TaskEnum::Move,
					};

					for (const auto& task : tasks) {
						m_taskList->AddTask(task);
					}
				}

				void NearSeekMove::DefineTask() {
					auto object = GetOwner()->GetGameObject();

					m_taskList->DefineTask(TaskEnum::Move, std::make_shared<TaskListNode::TargetSeek>(object, m_paramPtr->moveParamPtr));
				}

				void NearSeekMove::RotationUpdate() {
					auto rotationController = m_rotationController.lock();
					auto targetManager = m_targetManager.lock();
					if (!rotationController || !targetManager || !targetManager->HasTarget()) {
						return;
					}

					rotationController->SetDirection(targetManager->CalcuToTargetVec());
				}

				bool NearSeekMove::IsEnd() {
					return m_taskList->IsEnd();
				}

			}
		}
	}
}