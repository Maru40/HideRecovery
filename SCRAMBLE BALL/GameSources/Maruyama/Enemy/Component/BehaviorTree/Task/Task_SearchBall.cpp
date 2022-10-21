/*!
@file Task_SearchBall.cpp
@brief Task_SearchBallなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "Task_SearchBall.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/TaskList/TaskList.h"
#include "Maruyama/TaskList/CommonTasks/Task_MovePositions.h"
#include "Maruyama/TaskList/CommonTasks/MoveAstar.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Enemy/ImpactMap/FieldImpactMap.h"

#include "Maruyama/Item/HideItem.h"
#include "Maruyama/StageObject/HidePlace.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Task {

				//--------------------------------------------------------------------------------------
				///	ボールを探すタスクパラメータ
				//--------------------------------------------------------------------------------------

				SearchBall_Parametor::SearchBall_Parametor() :
					moveAstarParam(new basecross::Task::MoveAstar::Parametor())
				{}

				SearchBall_Parametor::~SearchBall_Parametor() {
					delete(moveAstarParam);
				}

				//--------------------------------------------------------------------------------------
				///	ボールを探すタスク
				//--------------------------------------------------------------------------------------

				SearchBall::SearchBall(const std::shared_ptr<Enemy::EnemyBase>& owner) :
					TaskBase(owner),
					m_param(Parametor()),
					m_taskList(new TaskList<TaskEnum>())
				{
					InitializeParametor();
					DefineTask();

					m_transform = GetOwner()->GetGameObject()->GetComponent<Transform>();
				}

				void SearchBall::OnStart() {
					SelectTask();					//タスクの選択

						//ターゲットの計算

					Debug::GetInstance()->Log(L"SearchStart");
				}

				bool SearchBall::OnUpdate() {
					m_taskList->UpdateTask();

					return IsEnd();
				}

				void SearchBall::OnExit() {
					m_taskList->ForceStop();

					Debug::GetInstance()->Log(L"SearchEnd");
				}

				void SearchBall::DefineTask() {
					auto ownerObject = GetOwner()->GetGameObject();

					//Astar移動
					m_taskList->DefineTask(
						TaskEnum::MoveAstar,
						std::make_shared<basecross::Task::MoveAstar>(GetOwner(), m_param.moveAstarParam)
					);

					//目的地についたら時の到着移動

				}

				void SearchBall::SelectTask() {
					TaskEnum tasks[] = {
						TaskEnum::MoveAstar,
						TaskEnum::MoveArrive,
					};

					for (const auto& task : tasks) {
						m_taskList->AddTask(task);
					}
				}

				void SearchBall::InitializeParametor() {
					m_param.moveAstarParam->movePositionsParam->moveParamPtr->speed = 10.0f;
				}

				bool SearchBall::IsEnd() const { return m_taskList->IsEnd(); }

			}
		}
	}
}