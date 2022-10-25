/*!
@file Task_SearchBall.cpp
@brief Task_SearchBall�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "Task_SearchBall.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Utility/Timer/GameTimer.h"
#include "Maruyama/Utility/Component/TargetManager.h"
#include "VelocityManager.h"

#include "Maruyama/TaskList/TaskList.h"
#include "Maruyama/TaskList/CommonTasks/Task_MovePositions.h"
#include "Maruyama/TaskList/CommonTasks/MoveAstar.h"
#include "Maruyama/TaskList/CommonTasks/Task_ToTargetMove.h"
#include "Maruyama/TaskList/CommonTasks/TargetSeek.h"
#include "Maruyama/TaskList/CommonTasks/Task_Wait.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Enemy/ImpactMap/FieldImpactMap.h"
#include "Maruyama/Enemy/AIDirector/EnemyAIDirector.h"

#include "Maruyama/Interface/I_FactionMember.h"
#include "Maruyama/Enemy/AIDirector/FactionCoordinator.h"
#include "Maruyama/Enemy/AIDirector/PatrolCoordinator.h"
#include "Maruyama/Enemy/AIDirector/PatrolCoordinator/HidePlacePatrol.h"

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
					moveAstarParam(new basecross::Task::MoveAstar::Parametor()),
					//toTargetMoveParam(new basecross::Task::ToTargetMove::Parametor()),
					targetSeekParam(new TaskListNode::TargetSeek::Parametor()),
					waitParam(new basecross::Task::Wait::Parametor(1.0f))
				{}

				SearchBall_Parametor::~SearchBall_Parametor() {
					delete(moveAstarParam);
					delete(targetSeekParam);
				}

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

					auto object = GetOwner()->GetGameObject();
					m_transform = object->GetComponent<Transform>();
					m_targetManager = object->GetComponent<TargetManager>();
					m_velocityManager = object->GetComponent<VelocityManager>();
					m_factionMember = object->GetComponent<Enemy::I_FactionMember>();
				}

				void SearchBall::OnStart() {
					SelectTask();		//�^�X�N�̑I��

					CalculateTarget();	//�^�[�Q�b�g�̌v�Z
				}

				bool SearchBall::OnUpdate() {
					m_taskList->UpdateTask();

					return IsEnd();
				}

				void SearchBall::OnExit() {
					m_taskList->ForceStop();
				}

				std::shared_ptr<GameObject> SearchBall::CalculateTarget() {
					using HidePlacePtrol = Enemy::AICoordinator::Patrol::HidePlacePatrol;

					auto targetManager = m_targetManager.lock();
					if (!targetManager) {	//�^�[�Q�b�g�Ǘ������݂��Ȃ��Ȃ珈�������Ȃ�
						return nullptr;
					}

					//�{����AIDirector�ɃA�N�Z�X���āA�^�[�Q�b�g���m�肳����B
					auto aiDirector = Enemy::AIDirector::GetInstance();
					auto factionMembmer = m_factionMember.lock();
					auto patrolCoordinator = factionMembmer->GetAssignedFaction<HidePlacePtrol>();	//�p�g���[���R�[�f�B�l�[�^�[�̎擾

					if (!patrolCoordinator) {	//�p�g���[�����łȂ������珈�����΂��B
						return nullptr;
					}

					//�p�g���[���R�[�f�B�l�[�^����^�[�Q�b�g���擾
					auto target = patrolCoordinator->SearchTarget(factionMembmer);
					targetManager->SetTarget(target);

					return target;
				}

				void SearchBall::DefineTask() {
					auto ownerObject = GetOwner()->GetGameObject();

					//Astar�ړ�
					m_taskList->DefineTask(
						TaskEnum::MoveAstar,
						std::make_shared<basecross::Task::MoveAstar>(GetOwner(), m_param.moveAstarParam)
					);

					//�ړI�n�̋߂��ɂ����玞�̓����ړ�
					m_taskList->DefineTask(
						TaskEnum::MoveArrive,
						std::make_shared<TaskListNode::TargetSeek>(ownerObject, m_param.targetSeekParam)
					);

					//�ҋ@
					m_param.waitParam->start = [&]() {	//�J�n�C�x���g
						if (auto velocityManager = m_velocityManager.lock()) {
							velocityManager->StartDeseleration();
						}
					};

					m_param.waitParam->exit = [&]() {	//�I���C�x���g
						if (auto velocityManager = m_velocityManager.lock()) {
							velocityManager->SetIsDeseleration(false);
						}
					};

					m_taskList->DefineTask(
						TaskEnum::Wait,
						std::make_shared<basecross::Task::Wait>(m_param.waitParam)
					);
				}

				void SearchBall::SelectTask() {
					TaskEnum tasks[] = {
						TaskEnum::MoveAstar,
						TaskEnum::MoveArrive,
						TaskEnum::Wait,
					};

					for (const auto& task : tasks) {
						m_taskList->AddTask(task);
					}
				}

				void SearchBall::InitializeParametor() {
					constexpr float MoveSpeed = 10.0f;
					constexpr float NearTargetRange = 1.5f;

					//Astar�ŖڕW�̋߂��܂ňړ�����p�����[�^
					m_param.moveAstarParam->movePositionsParam->moveParamPtr->speed = MoveSpeed;

					//�^�[�Q�b�g���[�u�̐ݒ�
					m_param.targetSeekParam->toTargetMoveParam->speed = MoveSpeed;
					m_param.targetSeekParam->toTargetMoveParam->moveType = basecross::Task::ToTargetMove::MoveType::ArriveVelocity;
					m_param.targetSeekParam->toTargetMoveParam->targetNearRange = NearTargetRange;
				}

				bool SearchBall::IsEnd() const { return m_taskList->IsEnd(); }

			}
		}
	}
}