/*!
@file ChaseState_AstarSeek.cpp
@brief ChaseState_AstarSeek�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/TaskList/TaskList.h"
#include "Maruyama/TaskList/CommonTasks/Task_MovePositions.h"
#include "Maruyama/TaskList/CommonTasks/Task_AroundEyeCheck.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Enemy/StateMachine/EnemyStateNodeBase.h"
#include "ChaseState_AstarSeek.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Enemy/ImpactMap/ImpactMap.h"

#include "Maruyama/Utility/Component/TargetManager.h"
#include "VelocityManager.h"
#include "Maruyama/Utility/Component/RotationController.h"

#include "Maruyama/Enemy/ImpactMap/FieldImpactMap.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			namespace ChaseState{

				//--------------------------------------------------------------------------------------
				/// �p�����[�^
				//--------------------------------------------------------------------------------------

				AstarSeekParametor::AstarSeekParametor() :
					AstarSeekParametor(
						std::make_shared<Task_MovePositions_Parametor>(),
						std::make_shared<Task::AroundEyeCheck_Parametor>())
				{}

				AstarSeekParametor::AstarSeekParametor(const std::shared_ptr<Task_MovePositions_Parametor> moveParamPtr,
					const std::shared_ptr<Task::AroundEyeCheck_Parametor> aroundEyeCheckParamPtr
				) :
					moveParamPtr(moveParamPtr), aroundEyeCheckParamPtr(aroundEyeCheckParamPtr)
				{}

				//--------------------------------------------------------------------------------------
				/// Astar���g�����Ǐ]�X�e�[�g�{��
				//--------------------------------------------------------------------------------------

				ChaseState_AstarSeek::ChaseState_AstarSeek(
					const std::shared_ptr<EnemyBase>& owner, 
					const std::shared_ptr<AstarSeekParametor>& paramPtr
				) :
					EnemyStateNodeBase<EnemyBase>(owner), 
					m_paramPtr(paramPtr), 
					m_taskList(new TaskList<TaskEnum>())
				{
					DefineTask();
				}

				void ChaseState_AstarSeek::OnStart() {
					m_paramPtr->moveParamPtr->positions = std::vector<Vec3>(); //���Z�b�g
					ReserveMoveStart(); //�ړ�����

					m_taskList->ForceStop(false);
					SelectTask();
				}

				bool ChaseState_AstarSeek::OnUpdate() {
					m_taskList->UpdateTask();
					Rotation();

					if (m_taskList->IsEnd()) { //�I�����ɌĂяo��
						EndProcess();
					}

					return false;
				}

				void ChaseState_AstarSeek::OnExit() {
					m_taskList->ForceStop();
				}


				void ChaseState_AstarSeek::DefineTask() {
					auto ownerObject = GetOwner()->GetGameObject();

					//�ړ�
					m_taskList->DefineTask(TaskEnum::Move, 
						std::make_shared<Task_MovePositions>(ownerObject, m_paramPtr->moveParamPtr));

					//���͂�ڂŊm�F����
					m_taskList->DefineTask(TaskEnum::AroundEyeCheck, 
						std::make_shared<Task::AroundEyeCheck>(ownerObject, m_paramPtr->aroundEyeCheckParamPtr));
				}

				void ChaseState_AstarSeek::SelectTask() {
					TaskEnum tasks[] = {
						TaskEnum::Move,
						TaskEnum::AroundEyeCheck,
					};

					for (const auto& task : tasks) {
						m_taskList->AddTask(task);
					}
				}

				void ChaseState_AstarSeek::ReserveMoveStart() {
					auto ownerObject = GetOwner()->GetGameObject();
					auto targetManager = ownerObject->GetComponent<TargetManager>(false);
					if (!targetManager) {
						return;
					}

					const auto& moveParamPtr = m_paramPtr->moveParamPtr;
					const auto& impactMap = maru::FieldImpactMap::GetInstance();
					auto selfPosition = ownerObject->GetComponent<Transform>()->GetPosition();
					auto targetPosition = targetManager->GetTargetPosition();
					moveParamPtr->positions = impactMap->GetRoutePositons(selfPosition, targetPosition);
				}

				void ChaseState_AstarSeek::Rotation() {
					if (m_taskList->IsEnd()) { //�I�����͌Ăяo���Ȃ�
						return;
					}
					if (m_taskList->GetCurrentTaskType() != TaskEnum::Move) {
						return;
					}

					auto ownerObject = GetOwner()->GetGameObject();
					auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);
					auto rotationController = ownerObject->GetComponent<RotationController>(false);
					if (!velocityManager || !rotationController) {
						return;
					}

					rotationController->SetDirection(velocityManager->GetVelocity());
				}

				void ChaseState_AstarSeek::EndProcess() {
					auto targetManager = GetOwner()->GetGameObject()->GetComponent<TargetManager>(false);
					if (targetManager) {
						targetManager->SetTarget(nullptr); //�^�[�Q�b�g����
					}
				}

				//--------------------------------------------------------------------------------------
				/// �A�N�Z�b�T
				//--------------------------------------------------------------------------------------

				bool ChaseState_AstarSeek::IsMoveEnd() {
					return m_taskList->IsEnd();
				}
			}
		}
	}
}