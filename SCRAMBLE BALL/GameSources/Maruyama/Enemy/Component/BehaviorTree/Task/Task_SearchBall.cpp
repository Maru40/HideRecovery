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
					movePositionsParam(new Task_MovePositions_Parametor())
				{}

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
					SelectTask();	//タスクの選択

					m_param.movePositionsParam->positions = CalculateMovePositions();	//徘徊移動先を設定

					Debug::GetInstance()->Log(L"SearchStart");
				}

				bool SearchBall::OnUpdate() {
					m_taskList->UpdateTask();

					return m_taskList->IsEnd();
				}

				void SearchBall::OnExit() {
					m_taskList->ForceStop();

					Debug::GetInstance()->Log(L"SearchEnd");
				}

				void SearchBall::DefineTask() {
					auto ownerObject = GetOwner()->GetGameObject();

					m_taskList->DefineTask(TaskEnum::Move, std::make_shared<Task_MovePositions>(ownerObject, m_param.movePositionsParam));
				}

				void SearchBall::SelectTask() {
					TaskEnum tasks[] = {
						TaskEnum::Move,
					};

					for (const auto& task : tasks) {
						m_taskList->AddTask(task);
					}
				}

				std::vector<Vec3> SearchBall::CalculateMovePositions() {
					//デバッグ
					//return { Vec3(0.0f, 1.0f, -6.0f), Vec3(5.0f, 1.0f, -6.0f), Vec3(-5.0f, 1.0f, -6.0f), Vec3(0.0f, 1.0f, -6.0f) };

					auto startPosition = m_transform.lock()->GetPosition();
					auto endPosition = CalculateMoveTargetPosition();

					auto areaIndex = FieldImpactMap::GetInstance()->SearchNearAreaIndex(startPosition);
					return FieldImpactMap::GetInstance()->GetRoutePositions(startPosition, endPosition, areaIndex);
				}

				Vec3 SearchBall::CalculateMoveTargetPosition() {
					auto hidePlace = maru::Utility::FindComponent<HidePlace>();

					auto position = hidePlace->GetGameObject()->GetComponent<Transform>()->GetPosition();
					Debug::GetInstance()->Log(position);
					return position;
				}

				void SearchBall::InitializeParametor() {

				}

			}
		}
	}
}