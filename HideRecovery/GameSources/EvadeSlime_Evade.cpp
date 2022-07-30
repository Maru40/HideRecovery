/*!
@file EvadeSlime_Evade.cpp
@brief EvadeSlime_Evadeのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyStateNodeBase.h"
#include "EvadeSlime_Evade.h"
//上記必要インクルード-----------------------------------------------------------------

#include "EnemyBase.h"
#include "TaskList.h"

#include "GameTimer.h"
#include "Task_Flee.h"

#include "Task_AroundEyeCheck.h"

#include "Task_ToTargetMove.h"
#include "Task_MovePositions.h"
#include "Task_AstarMove.h"

#include "DebugObject.h"

namespace basecross {
	namespace Enemy {

		namespace EvadeSlime {

			namespace StateNode {

				//--------------------------------------------------------------------------------------
				/// パラメータ
				//--------------------------------------------------------------------------------------

				Evade_Parametor::Evade_Parametor() :
					Evade_Parametor(
						std::make_shared<Enemy::Task::Flee::Parametor>(6.0f, 2.0f),
						std::make_shared<basecross::Task::AstarMove_Parametor>(
							basecross::Task::AstarMove::MoveType::Evade,
							std::make_shared<Task_MovePositions_Parametor>(
								Task_ToTargetMove::Parametor(6.0f, 2.0f, Task_ToTargetMove::MoveType::SeekVelocity)
							)
						),
						std::make_shared<basecross::Task::AroundEyeCheck::Parametor>(2.0f, 60.0f)
					)
				{}

				Evade_Parametor::Evade_Parametor(
					const std::shared_ptr<Enemy::Task::Flee_Parametor>& fleeParamPtr,
					const std::shared_ptr<basecross::Task::AstarMove_Parametor>& astarMoveParamPtr,
					const std::shared_ptr<basecross::Task::AroundEyeCheck::Parametor>& aroundEyeCheckParamPtr
				) :
					fleeParamPtr(fleeParamPtr),
					astarMoveParamPtr(astarMoveParamPtr),
					aroundEyeCheckParamPtr(aroundEyeCheckParamPtr)
				{}

				//--------------------------------------------------------------------------------------
				/// 逃げるステート本体
				//--------------------------------------------------------------------------------------

				Evade::Evade(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor> paramPtr)
					:EnemyStateNodeBase<EnemyBase>(owner), m_paramPtr(paramPtr), m_taskList(new TaskList<TaskEnum>())
				{
					DefineTask();
				}

				void Evade::OnStart() {
					m_taskList->ForceStop(false);
					SelectTask();

					DebugObject::AddString(L"EvadeStart");
				}

				bool Evade::OnUpdate() {
					m_taskList->UpdateTask();

					if (m_taskList->IsEnd()) { //終了時に呼び出す処理
						m_taskList->AddTask(TaskEnum::AstarEvade);
					}

					return false;
				}

				void Evade::OnExit() {
					m_taskList->ForceStop(true);
				}


				void Evade::DefineTask() {
					auto enemy = GetOwner();

					//直感的に逃げる行為
					m_taskList->DefineTask(TaskEnum::Flee, 
						std::make_shared<Enemy::Task::Flee>(enemy, m_paramPtr->fleeParamPtr));

					//地形情報を元に逃げる行為
					m_taskList->DefineTask(TaskEnum::AstarEvade,
						std::make_shared<basecross::Task::AstarMove>(enemy->GetGameObject(), m_paramPtr->astarMoveParamPtr));

					//周りを確認する行動
					m_taskList->DefineTask(TaskEnum::AroundEyeCheck, 
						std::make_shared<basecross::Task::AroundEyeCheck>(enemy->GetGameObject(), m_paramPtr->aroundEyeCheckParamPtr));
				}

				void Evade::SelectTask() {
					TaskEnum tasks[] = {
						TaskEnum::Flee,
						TaskEnum::AstarEvade,
						//TaskEnum::AroundEyeCheck,
					};

					for (const auto& task : tasks) {
						m_taskList->AddTask(task);
					}
				}
			
			}
		}
	}
}