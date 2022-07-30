
/*!
@file Slime_Attack.cpp
@brief Slime_Attack�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "TaskList.h"
#include "GameTimer.h"
#include "Task_Preliminary_NormalSlime.h"
#include "Task_BodyBlow.h"
#include "Task_Wait.h"

#include "EnemyStateNodeBase.h"

#include "Slime_Attack.h"

#include "TargetManager.h"
#include "EnemyBase.h"
#include "I_Damaged.h"
#include "EnemyMainStateMachine.h"
#include "StatorBase.h"
#include "SlimeStatorBase.h"
#include "Stator_NormalSlime.h"

#include "VelocityManager.h"
#include "CollisionAction.h"

#include "DebugObject.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// �p�����[�^
			//--------------------------------------------------------------------------------------

			Slime_Attack_Parametor::Slime_Attack_Parametor():
				preliminaryParamPtr(new Task_Preliminary_NormalSlime::Parametor()),
				bodyBlowParamPtr(new Task_BodyBlow::Parametor()), 
				waitParamPtr(new Task::Wait::Parametor(2.0f))
			{}

			//--------------------------------------------------------------------------------------
			/// �U���X�e�[�g�{��
			//--------------------------------------------------------------------------------------

			Slime_Attack::Slime_Attack(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr)
				:EnemyStateNodeBase<EnemyBase>(owner), m_paramPtr(paramPtr), m_taskList(new TaskList<TaskEnum>())
			{
				DefineTask();
			}

			void Slime_Attack::OnStart() {
				m_taskList->ForceStop(false);
				SelectTask();
			}

			bool Slime_Attack::OnUpdate() {
				m_taskList->UpdateTask();

				return m_taskList->IsEnd();
			}

			void Slime_Attack::OnExit() {
				m_taskList->ForceStop(true);
			}


			void Slime_Attack::DefineTask() {
				auto ownerObject = GetOwner()->GetGameObject();
				auto enemy = ownerObject->GetComponent<EnemyBase>();
				auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);

				//�\������
				m_taskList->DefineTask(TaskEnum::Preliminary, std::make_shared<Task_Preliminary_NormalSlime>(enemy, m_paramPtr->preliminaryParamPtr));

				//�ːi
				m_taskList->DefineTask(TaskEnum::BodyBlow, std::make_shared<Task_BodyBlow>(enemy, m_paramPtr->bodyBlowParamPtr));

				//�ҋ@
				auto& waitParamPtr = m_paramPtr->waitParamPtr;
				waitParamPtr->exit = [&, this]() { GetOwner()->GetGameObject()->GetComponent<VelocityManager>()->SetIsDeseleration(false); };  //�����������I������B
				m_taskList->DefineTask(TaskEnum::Wait, std::make_shared<Task::Wait>(waitParamPtr));
			}

			void Slime_Attack::SelectTask() {
				TaskEnum tasks[] = {
					TaskEnum::Preliminary,
					TaskEnum::BodyBlow,
					TaskEnum::Wait,
				};

				for (const auto& task : tasks) {
					m_taskList->AddTask(task);
				}
			}

			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			Slime_Attack::TaskEnum Slime_Attack::GetCurrentTask() const {
				return m_taskList->GetCurrentTaskType();
			}
		}
	}
}