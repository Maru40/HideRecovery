
/*!
@file Slime_KnockBack.cpp
@brief Slime_KnockBack�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyStateNodeBase.h"
#include "Slime_KnockBack.h"

#include "EnemyBase.h"
#include "TaskList.h"

#include "GameTimer.h"
#include "Task_Wait.h"

#include "VelocityManager.h"

#include "DebugObject.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// �p�����[�^
			//--------------------------------------------------------------------------------------

			Slime_KnockBack_Parametor::Slime_KnockBack_Parametor()
				:Slime_KnockBack_Parametor(std::make_shared<basecross::Task::Wait_Parametor>(1.0f))
			{}

			Slime_KnockBack_Parametor::Slime_KnockBack_Parametor(
				const std::shared_ptr<basecross::Task::Wait_Parametor>& waitParamPtr
			):
				waitParamPtr(waitParamPtr)
			{}

			//--------------------------------------------------------------------------------------
			/// �X���C���̃m�b�N�o�b�N�X�e�[�g�{��
			//--------------------------------------------------------------------------------------

			Slime_KnockBack::Slime_KnockBack(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr)
				:StateNodeBase(owner), m_paramPtr(paramPtr), m_taskList(new TaskList<TaskEnum>())
			{
				DefineTask();
			}

			void Slime_KnockBack::OnStart() {
				m_taskList->ForceStop(false);
				if (m_paramPtr->startAnimationFunction) {
					m_paramPtr->startAnimationFunction();
				}

				SelectTask(); 

				//���x��Over��h��
				auto velocityManager = GetOwner()->GetGameObject()->GetComponent<VelocityManager>(false);
				if (velocityManager && velocityManager->GetVelocity().length() > m_paramPtr->maxPower) {
					auto velocity = velocityManager->GetVelocity();
					velocityManager->SetVelocity(velocity.GetNormalized() * m_paramPtr->maxPower);
				}
			}

			bool Slime_KnockBack::OnUpdate() {
				m_taskList->UpdateTask();

				return m_taskList->IsEnd();
			}

			void Slime_KnockBack::OnExit() {
				auto velocityManager = GetOwner()->GetGameObject()->GetComponent<VelocityManager>(false);
				if (velocityManager) {
					velocityManager->SetIsDeseleration(false);
				}
			}


			void Slime_KnockBack::DefineTask() {
				auto velocityManager = GetOwner()->GetGameObject()->GetComponent<VelocityManager>(false);

				constexpr float notDeseleratioinTime = 0.125f; //�������Ȃ�����
				m_taskList->DefineTask(TaskEnum::NotDeselerationWait, std::make_shared<Task::Wait>(notDeseleratioinTime));

				m_paramPtr->waitParamPtr->start =
					[&, velocityManager]() { velocityManager ? velocityManager->StartDeseleration() : nullptr; };
				m_taskList->DefineTask(TaskEnum::Wait, std::make_shared<Task::Wait>(m_paramPtr->waitParamPtr));
			}

			void Slime_KnockBack::SelectTask() {
				m_taskList->AddTask(TaskEnum::NotDeselerationWait);
				m_taskList->AddTask(TaskEnum::Wait);
			}

		}
	}
}