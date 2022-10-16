/*!
@file Task_SearchBall.cpp
@brief Task_SearchBall‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "Task_SearchBall.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/TaskList/TaskList.h"
#include "Maruyama/TaskList/CommonTasks/Task_MovePositions.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Enemy/ImpactMap/FieldImpactMap.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Task {

				SearchBall::SearchBall(const std::shared_ptr<Enemy::EnemyBase>& owner) :
					TaskBase(owner),
					m_taskList(new TaskList<TaskEnum>())
				{
					InitializeParametor();
					DefineTask();

					m_transform = GetOwner()->GetGameObject()->GetComponent<Transform>();
				}

				void SearchBall::OnStart() {
					SelectTask();	//ƒ^ƒXƒN‚Ì‘I‘ð

					m_param.movePositionsParam->positions = CalculateMovePositions();	//œpœjˆÚ“®æ‚ðÝ’è
				}

				bool SearchBall::OnUpdate() {
					m_taskList->UpdateTask();

					return m_taskList->IsEnd();
				}

				void SearchBall::OnExit() {
					m_taskList->ForceStop();
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
					auto startPosition = m_transform.lock()->GetPosition();
					auto endPosition = CalculateMoveTargetPosition();

					return FieldImpactMap::GetInstance()->GetRoutePositons(startPosition, endPosition);
				}

				Vec3 SearchBall::CalculateMoveTargetPosition() {
					return Vec3(0.0f);
				}

				void SearchBall::InitializeParametor() {

				}

			}
		}
	}
}