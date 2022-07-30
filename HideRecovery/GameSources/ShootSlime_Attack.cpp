/*!
@file ShootSlime_Attack.cpp
@brief ShootSlime_Attack‚ÌƒNƒ‰ƒXÀ‘Ì
’S“–FŠÛR—TŠì
*/

#include "stdafx.h"
#include "Project.h"

#include "TaskList.h"
#include "GameTimer.h"

#include "Task_Wait.h"
#include "ShootManager.h"
#include "Task_Preliminary_Shoot.h"
#include "Task_Shoot.h"

#include "EnemyStateNodeBase.h"

#include "ShootSlime_Attack.h"

#include "EnemyBase.h"

#include "EnemyMainStateMachine.h"

#include "ShootManager.h"

namespace basecross {
	namespace Enemy {

		namespace ShootSlime {

			namespace StateNode {

				//--------------------------------------------------------------------------------------
				/// ƒpƒ‰ƒ[ƒ^
				//--------------------------------------------------------------------------------------

				Attack_Parametor::Attack_Parametor()
					:Attack_Parametor(
						std::make_shared<Task::Preliminary_Shoot_Parametor>(std::make_shared<ShootChargeData>(2.0f, 5.0f)),
						std::make_shared<Task::Shoot_Parametor>(),
						std::make_shared<basecross::Task::Wait::Parametor>(1.0f)
					)
				{}

				Attack_Parametor::Attack_Parametor(
					const std::shared_ptr<Task::Preliminary_Shoot_Parametor>& preliminaryParamPtr,
					const std::shared_ptr<Task::Shoot_Parametor>& shootParamPtr,
					const std::shared_ptr<basecross::Task::Wait_Parametor>& waitParamPtr
				) :
					preliminaryParamPtr(preliminaryParamPtr),
					shootParamPtr(shootParamPtr),
					waitParamPtr(waitParamPtr)
				{}

				//--------------------------------------------------------------------------------------
				/// UŒ‚ƒXƒe[ƒg–{‘Ì
				//--------------------------------------------------------------------------------------

				Attack::Attack(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr)
					:EnemyStateNodeBase(owner), m_paramPtr(paramPtr), m_taskList(new TaskList<TaskEnum>())
				{
					DefineTask();
				}

				void Attack::OnStart() {
					m_taskList->ForceStop(false);
					SelectTask();
				}

				bool Attack::OnUpdate() {
					m_taskList->UpdateTask();

					return m_taskList->IsEnd();
				}

				void Attack::OnExit() {
					m_taskList->ForceStop(true);
				}


				void Attack::DefineTask() {
					auto enemy = GetOwner();

					//—\”õ“®ì
					m_taskList->DefineTask(TaskEnum::Preliminary, std::make_shared<Task::Preliminary_Shoot>(enemy, m_paramPtr->preliminaryParamPtr));

					//UŒ‚
					m_taskList->DefineTask(TaskEnum::Shoot, std::make_shared<Task::Shoot>(enemy, m_paramPtr->shootParamPtr));

					//UŒ‚Œãd’¼
					m_taskList->DefineTask(TaskEnum::Wait, std::make_shared<basecross::Task::Wait>(m_paramPtr->waitParamPtr));

					//’e‚ÌƒŠƒ[ƒh
					m_taskList->DefineTask(TaskEnum::Reload, 
						[&, this]() { GetOwner()->GetGameObject()->GetComponent<ShootManager>()->Reload(); },
						[]() { return true; }, 
						[]() {}
					);
				}

				void Attack::SelectTask() {
					TaskEnum tasks[] = {
						TaskEnum::Preliminary,
						TaskEnum::Shoot,
						TaskEnum::Wait,
						TaskEnum::Reload,
					};

					for (const auto& task : tasks) {
						m_taskList->AddTask(task);
					}
				}

			}
		}
	}
}