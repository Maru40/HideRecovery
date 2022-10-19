/*!
@file Task_SearchBall.cpp
@brief Task_SearchBall�Ȃǎ���
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
				///	�{�[����T���^�X�N�p�����[�^
				//--------------------------------------------------------------------------------------

				SearchBall_Parametor::SearchBall_Parametor() :
					movePositionsParam(new Task_MovePositions_Parametor())
				{}

				//--------------------------------------------------------------------------------------
				///	�{�[����T���^�X�N
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
					SelectTask();	//�^�X�N�̑I��

					CalculateMoveAreaRouteQueue();	//�p�j�G���A���[�g�̎擾
					m_param.movePositionsParam->positions = CalculateMovePositions();	//�p�j�ړ����ݒ�

					Debug::GetInstance()->Log(L"SearchStart");
				}

				bool SearchBall::OnUpdate() {
					m_taskList->UpdateTask();

					if (m_taskList->IsEnd()) {
						NextRoute();
					}

					return IsEnd();
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

				void SearchBall::NextRoute() {
					if (m_areaRoute.empty()) {
						return;
					}

					m_param.movePositionsParam->positions = CalculateMovePositions();	//�V�����|�W�V�����ɕύX

					SelectTask();				//�^�X�N�̍Ďn��
				}

				std::queue<int> SearchBall::CalculateMoveAreaRouteQueue() {
					maru::Utility::QueueClear(m_areaRoute);

					auto startPosition = m_transform.lock()->GetPosition();
					auto targetPosition = CalculateMoveTargetPosition();

					auto routes = FieldImpactMap::GetInstance()->SearchAreaIndexRoute(startPosition, targetPosition);

					m_areaRoute.push(FieldImpactMap::GetInstance()->SearchNearAreaIndex(startPosition));	//�ŏ��̎������g�̃m�[�h���Ȃ����߁B
					for (const auto& route : routes) {
						m_areaRoute.push(route);
					}

					return m_areaRoute;
				}

				std::vector<Vec3> SearchBall::CalculateMovePositions() {
					if (m_areaRoute.empty()) {
						return std::vector<Vec3>();
					}

					auto startPosition = m_transform.lock()->GetPosition();
					auto endPosition = CalculateMoveTargetPosition();

					int areaIndex = m_areaRoute.front();
					m_areaRoute.pop();
					return FieldImpactMap::GetInstance()->GetRoutePositions(startPosition, endPosition, areaIndex);
				}

				Vec3 SearchBall::CalculateMoveTargetPosition() {
					auto hidePlace = maru::Utility::FindComponent<HidePlace>();

					auto position = hidePlace->GetGameObject()->GetComponent<Transform>()->GetPosition();
					Debug::GetInstance()->Log(position);
					return position;
				}

				void SearchBall::InitializeParametor() {
					//m_param.movePositionsParam->moveParamPtr->speed = 10.0f;
				}

				bool SearchBall::IsEnd() const { return m_areaRoute.empty() && m_taskList->IsEnd(); }

			}
		}
	}
}