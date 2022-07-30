
/*!
@file BigShootSlime_Attack.cpp
@brief BigShootSlime_Attackのクラス実体
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

#include "BigShootSlime_Attack.h"

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

#include "StatorBase.h"
#include "Stator_ShootSlime.h"

#include "AbsorbedSlimeManager.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// パラメータ
			//--------------------------------------------------------------------------------------

			BigShootSlime_Attack_Parametor::BigShootSlime_Attack_Parametor() {}

			//--------------------------------------------------------------------------------------
			/// 巨大シュートスライムの攻撃ステート本体
			//--------------------------------------------------------------------------------------

			BigShootSlime_Attack::BigShootSlime_Attack(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr)
				:EnemyStateNodeBase(owner), m_paramPtr(paramPtr), m_taskList(new TaskList<TaskEnum>)
			{
				DefineTask(); //タスクの定義
			}

			void BigShootSlime_Attack::OnStart() {
				auto velocityManager = GetOwner()->GetGameObject()->GetComponent<VelocityManager>(false);
				if (velocityManager) {
					velocityManager->StartDeseleration();
				}

				m_taskList->ForceStop(false);
				SelectTask();
			}

			bool BigShootSlime_Attack::OnUpdate() {
				m_taskList->UpdateTask();
				return m_taskList->IsEnd();
			}

			void BigShootSlime_Attack::OnExit() {
				m_taskList->ForceStop(true);

				auto velocityManager = GetOwner()->GetGameObject()->GetComponent<VelocityManager>(false);
				if (velocityManager) {
					velocityManager->SetIsDeseleration(false);
				}

				auto absorbed = GetOwner()->GetGameObject()->GetComponent<BigSlime::AbsorbedSlimeManagerBase>(false);
				if (absorbed) {
					absorbed->ChangeStates<ShootSlime::Stator_ShootSlime>(ShootSlime::Stator_ShootSlime::State::Absorbed);
				}
			}

			void BigShootSlime_Attack::DefineTask() {
				auto ownerObject = GetOwner()->GetGameObject();
				auto enemy = ownerObject->GetComponent<EnemyBase>();
				auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);
			}

			void BigShootSlime_Attack::SelectTask() {
				TaskEnum tasks[] = {
					TaskEnum::Preliminary,
					TaskEnum::BodyBlow,
					TaskEnum::Wait,
				};

				for (const auto& task : tasks) {
					m_taskList->AddTask(task);
				}
			}

			bool BigShootSlime_Attack::IsEnd() const {
				auto absorbed = GetOwner()->GetGameObject()->GetComponent<BigSlime::AbsorbedSlimeManagerBase>(false);
				if (!absorbed) {
					return true;
				}

				return true;
			}
		}
	}
}