/*!
@file ChaseState_AstarSeek.cpp
@brief ChaseState_AstarSeekのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "TaskList.h"
#include "Task_MovePositions.h"
#include "Task_AroundEyeCheck.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Enemy/StateMachine/EnemyStateNodeBase.h"
#include "ChaseState_AstarSeek.h"

#include "SingletonComponent.h"
#include "ImpactMap.h"

#include "TargetManager.h"
#include "VelocityManager.h"
#include "RotationController.h"

#include "Maruyama/Enemy/Component/ImpactMap/FieldImpactMap.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			namespace ChaseState{

				//--------------------------------------------------------------------------------------
				/// パラメータ
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
				/// Astarを使った追従ステート本体
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
					m_paramPtr->moveParamPtr->positions = std::vector<Vec3>(); //リセット
					ReserveMoveStart(); //移動準備

					m_taskList->ForceStop(false);
					SelectTask();
				}

				bool ChaseState_AstarSeek::OnUpdate() {
					m_taskList->UpdateTask();
					Rotation();

					if (m_taskList->IsEnd()) { //終了時に呼び出す
						EndProcess();
					}

					return false;
				}

				void ChaseState_AstarSeek::OnExit() {
					m_taskList->ForceStop();
				}


				void ChaseState_AstarSeek::DefineTask() {
					auto ownerObject = GetOwner()->GetGameObject();

					//移動
					m_taskList->DefineTask(TaskEnum::Move, 
						std::make_shared<Task_MovePositions>(ownerObject, m_paramPtr->moveParamPtr));

					//周囲を目で確認する
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
					if (m_taskList->IsEnd()) { //終了時は呼び出さない
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

					rotationController->SetDirect(velocityManager->GetVelocity());
				}

				void ChaseState_AstarSeek::EndProcess() {
					auto targetManager = GetOwner()->GetGameObject()->GetComponent<TargetManager>(false);
					if (targetManager) {
						targetManager->SetTarget(nullptr); //ターゲット消失
					}
				}

				//--------------------------------------------------------------------------------------
				/// アクセッサ
				//--------------------------------------------------------------------------------------

				bool ChaseState_AstarSeek::IsMoveEnd() {
					return m_taskList->IsEnd();
				}
			}
		}
	}
}