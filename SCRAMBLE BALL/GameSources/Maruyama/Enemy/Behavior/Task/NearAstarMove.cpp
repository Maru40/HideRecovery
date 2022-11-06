/*!
@file NearAstarMove.cpp
@brief NearAstarMove�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "NearAstarMove.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/TaskList/TaskList.h"
#include "Maruyama/TaskList/CommonTasks/MoveAstar.h"
#include "Maruyama/TaskList/CommonTasks/Task_MovePositions.h"
#include "Maruyama/TaskList/CommonTasks/Task_ToTargetMove.h"

#include "Maruyama/Enemy/Component/EyeSearchRange.h"
#include "Maruyama/Utility/Component/RotationController.h"
#include "Maruyama/Utility/Component/TargetManager.h"

#include "Maruyama/TaskList/CommonTasks/Task_ToTargetMove.h"

#include "Maruyama/Utility/Utility.h"
#include "Maruyama/Utility/UtilityObstacle.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Task {

				//--------------------------------------------------------------------------------------
				///	�^�[�Q�b�g��Astar�ŋ߂Â��p�����[�^
				//--------------------------------------------------------------------------------------

				NearAstarMove_Parametor::NearAstarMove_Parametor():
					NearAstarMove_Parametor(EyeSearchRangeParametor())
				{}

				NearAstarMove_Parametor::NearAstarMove_Parametor(const EyeSearchRangeParametor& eyeParam):
					eyeSearchParamPtr(new EyeSearchRangeParametor(eyeParam)),
					moveParamPtr(new basecross::Task::MoveAstar_Parametor())
				{}

				NearAstarMove_Parametor::~NearAstarMove_Parametor() {
					delete(eyeSearchParamPtr);
					delete(moveParamPtr);
				}

				//--------------------------------------------------------------------------------------
				///	�^�[�Q�b�g��Astar�ŋ߂Â�
				//--------------------------------------------------------------------------------------

				NearAstarMove::NearAstarMove(const std::shared_ptr<Enemy::EnemyBase>& owner, const Parametor* paramPtr):
					TaskBase(owner),
					m_paramPtr(paramPtr),
					m_taskList(new TaskList<TaskEnum>())
				{
					auto object = owner->GetGameObject();

					m_eyeRange = object->GetComponent<EyeSearchRange>(false);
					m_targetManager = object->GetComponent<TargetManager>(false);
					m_rotationController = object->GetComponent<RotationController>(false);

					DefineTask();
				}

				void NearAstarMove::OnStart() {
					SelectTask();
				}

				bool NearAstarMove::OnUpdate() {
					m_taskList->UpdateTask();
					RotationUpdate();

					return IsEnd();
				}

				void NearAstarMove::OnExit() {
					m_taskList->ForceStop();
				}

				void NearAstarMove::DefineTask() {
					auto enemy = GetOwner();

					m_taskList->DefineTask(TaskEnum::Move, std::make_shared<basecross::Task::MoveAstar>(enemy, m_paramPtr->moveParamPtr));
				}

				void NearAstarMove::SelectTask() {
					TaskEnum tasks[] = {
						TaskEnum::Move,
					};

					for (const auto& task : tasks) {
						m_taskList->AddTask(task);
					}
				}

				void NearAstarMove::RotationUpdate() {
					auto rotationController = m_rotationController.lock();
					if (!rotationController) {
						return;
					}

					auto endPosition = m_paramPtr->moveParamPtr->movePositionsParam->moveParamPtr->endPosition;
					auto direction = endPosition - GetOwner()->GetGameObject()->GetComponent<Transform>()->GetPosition();

					rotationController->SetDirection(direction);
				}

				bool NearAstarMove::IsEnd() const {
					//�^�[�Q�b�g�����݂��Ȃ��Ȃ�I��
					auto targetManager = m_targetManager.lock();
					if (!targetManager || !targetManager->HasTarget()) {
						return true;
					}

					auto target = targetManager->GetTarget();

					//�^�[�Q�b�g���͈͓��A���A��Q�����Ȃ��Ȃ�
					if (m_eyeRange.lock()->IsInEyeRange(target, *m_paramPtr->eyeSearchParamPtr)) {
						return true;
					}

					return  m_taskList->IsEnd();
				}

			}
		}
	}
}