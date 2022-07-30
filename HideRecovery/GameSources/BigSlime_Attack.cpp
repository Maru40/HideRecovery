
/*!
@file Slime_Attack.cpp
@brief Slime_Attackのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "PressData.h"
#include "TaskList.h"
#include "GameTimer.h"
#include "Task_Preliminary_BigSlime.h"
#include "Task_BodyBlow.h"
#include "Task_BodyBlow_BigSlime.h"
#include "Task_Wait.h"

#include "EnemyStateNodeBase.h"

#include "BigSlime_Attack.h"

#include "EnemyBase.h"

#include "AbsorbedSlimeManager.h"

#include "EnemyObjectBase.h"
#include "SlimeObjectBase.h"

#include "EnemyMainStateMachine.h"
#include "StatorBase.h"
#include "SlimeStatorBase.h"
#include "Stator_NormalSlime.h"
#include "Stator_BigSlime.h"

#include "VelocityManager.h"

#include "RotationController.h"
#include "ToTargetMove.h"

#include "I_Damaged.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// パラメータ
			//--------------------------------------------------------------------------------------

			BigSlime_Attack_Parametor::BigSlime_Attack_Parametor():
				preliminaryParamPtr(new BigSlime::Task::Preliminary_Attack_Parametor()),
				bodyBlowParamPtr(new BigSlime::Task::BodyBlow::Parametor()),
				waitParamPtr(new Task::Wait::Parametor(2.0f))
			{}

			//--------------------------------------------------------------------------------------
			/// 巨大スライムの攻撃ステート本体
			//--------------------------------------------------------------------------------------

			BigSlime_Attack::BigSlime_Attack(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr)
				:EnemyStateNodeBase(owner), m_paramPtr(paramPtr), m_taskList(new TaskList<TaskEnum>)
			{
				DefineTask();
			}

			void BigSlime_Attack::OnStart() {
				auto velocityManager = GetOwner()->GetGameObject()->GetComponent<VelocityManager>(false);
				if (velocityManager) {
					velocityManager->StartDeseleration();
				}

				m_taskList->ForceStop(false);
				SelectTask();
			}

			bool BigSlime_Attack::OnUpdate() {
				m_taskList->UpdateTask();
				return m_taskList->IsEnd();
			}

			void BigSlime_Attack::OnExit() {
				m_taskList->ForceStop(true);

				auto velocityManager = GetOwner()->GetGameObject()->GetComponent<VelocityManager>(false);
				if (velocityManager) {
					velocityManager->SetIsDeseleration(false);
				}
			}

			void BigSlime_Attack::DefineTask() {
				auto ownerObject = GetOwner()->GetGameObject();
				auto enemy = ownerObject->GetComponent<EnemyBase>();
				auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);

				//予備動作
				m_taskList->DefineTask(TaskEnum::Preliminary, std::make_shared<BigSlime::Task::Preliminary_Attack>(enemy, m_paramPtr->preliminaryParamPtr));

				//BodyBlow
				m_taskList->DefineTask(TaskEnum::BodyBlow, std::make_shared<BigSlime::Task::BodyBlow>(enemy, m_paramPtr->bodyBlowParamPtr));

				//Wait
				auto& waitParamPtr = m_paramPtr->waitParamPtr;
				m_taskList->DefineTask(TaskEnum::Wait, std::make_shared<Task::Wait>(waitParamPtr));
			}

			void BigSlime_Attack::SelectTask() {
				TaskEnum tasks[] = {
					TaskEnum::Preliminary,
					TaskEnum::BodyBlow,
					TaskEnum::Wait,
				};

				for (const auto& task : tasks) {
					m_taskList->AddTask(task);
				}
			}

		}
	}
}