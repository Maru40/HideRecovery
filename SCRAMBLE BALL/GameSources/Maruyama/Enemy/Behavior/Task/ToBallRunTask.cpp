/*!
@file ToBallRunTask.cpp
@brief ToBallRunTask�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "ToBallRunTask.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Utility/Timer/GameTimer.h"
#include "Maruyama/Utility/Component/TargetManager.h"
#include "VelocityManager.h"

#include "Maruyama/TaskList/TaskList.h"
#include "Maruyama/TaskList/CommonTasks/Task_MovePositions.h"
#include "Maruyama/TaskList/CommonTasks/MoveAstar.h"
#include "Maruyama/TaskList/CommonTasks/Task_ToTargetMove.h"
#include "Maruyama/TaskList/CommonTasks/TargetSeek.h"
#include "Maruyama/TaskList/CommonTasks/OpenBox.h"
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

#include "Maruyama/Enemy/Component/EyeSearchRange.h"
#include "Maruyama/Utility/Component/RotationController.h"
#include "Maruyama/Utility/UtilityObstacle.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"
#include "Maruyama/Item/HideItemObject.h"

#include "Itabashi/OnlinePlayerSynchronizer.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Task {

				//--------------------------------------------------------------------------------------
				///	�{�[���֌������^�X�N�̃p�����[�^
				//--------------------------------------------------------------------------------------

				ToBallRunTask_Parametor::ToBallRunTask_Parametor() :
					moveAstarParam(new basecross::Task::MoveAstar::Parametor()),
					targetSeekParam(new TaskListNode::TargetSeek::Parametor()),
					waitParam(new basecross::Task::Wait::Parametor(0.5f))
				{}

				ToBallRunTask_Parametor::~ToBallRunTask_Parametor() {
					delete(moveAstarParam);
					delete(targetSeekParam);
				}

				//--------------------------------------------------------------------------------------
				///	�{�[���֌������^�X�N�̖{��
				//--------------------------------------------------------------------------------------

				ToBallRunTask::ToBallRunTask(const std::shared_ptr<Enemy::EnemyBase>& owner):
					TaskBase(owner),
					m_param(Parametor()),
					m_taskList(new TaskList<TaskEnum>())
				{
					InitializeParametor();
					DefineTask();

					auto object = GetOwner()->GetGameObject();
					m_transform = object->GetComponent<Transform>();
					m_eyeRange = object->GetComponent<EyeSearchRange>();
					m_targetManager = object->GetComponent<TargetManager>();
					m_velocityManager = object->GetComponent<VelocityManager>();
					m_rotationController = object->GetComponent<RotationController>();
					m_factionMember = object->GetComponent<Enemy::I_FactionMember>();
					m_onlineSynchronizer = object->GetComponent<OnlinePlayerSynchronizer>();
				}

				void ToBallRunTask::OnStart() {
					CalculateTarget();	//�^�[�Q�b�g�̌v�Z

					SelectTask();		//�^�X�N�̑I��
				}

				bool ToBallRunTask::OnUpdate() {
					m_taskList->UpdateTask();
					Rotation();
					CheckForceNextMoveArriveTask();

					return IsEnd();
				}

				void ToBallRunTask::OnExit() {
					m_taskList->ForceStop();
				}

				std::shared_ptr<GameObject> ToBallRunTask::CalculateTarget() {
					auto hideObjects = ShareClassesManager::GetInstance()->GetCastShareClasses<HideItemObject>();

					m_targetManager.lock()->SetTarget(hideObjects[0].lock());
					return hideObjects[0].lock();
				}

				void ToBallRunTask::CheckForceNextMoveArriveTask() {
					if (m_taskList->IsEnd() || !m_targetManager.lock()->HasTarget()) {
						return;
					}

					//Astar�ړ��łȂ������珈�������Ȃ�
					if (m_taskList->GetCurrentTaskType() != TaskEnum::MoveAstar) {
						return;
					}

					//�^�[�Q�b�g����Q�����̏�ԂȂ�
					auto startPosition = m_transform.lock()->GetPosition();
					auto targetPosition = m_targetManager.lock()->GetTargetPosition();
					auto toTargetPosition = targetPosition - startPosition;

					//�^�[�Q�b�g�̋��������E�������傫��������A���������Ȃ��B
					if (m_eyeRange.lock()->GetEyeLength() < toTargetPosition.length()) {
						return;
					}

					//Ray��������B
					auto objects = GetOwner()->GetGameObject()->GetStage()->GetGameObjectVec();
					if (!maru::UtilityObstacle::IsRayObstacle(startPosition, targetPosition, objects)) {
						m_taskList->ForceNextTask();
					}
				}

				void ToBallRunTask::DefineTask() {
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

					//�ҋ@�s��
					DefineWaitTask();
				}

				void ToBallRunTask::DefineWaitTask() {
					//�J�n�C�x���g
					m_param.waitParam->start = [&]() {
						m_onlineSynchronizer.lock()->Aquisition();
					};

					//�I���C�x���g
					m_param.waitParam->exit = [&]() {
						
					};

					//�^�X�N�̒�`
					m_taskList->DefineTask(
						TaskEnum::GetBall,
						std::make_shared<basecross::Task::Wait>(m_param.waitParam)
					);
				}

				void ToBallRunTask::SelectTask() {
					TaskEnum tasks[] = {
						TaskEnum::MoveAstar,
						TaskEnum::MoveArrive,
						//TaskEnum::GetBall,
					};

					for (const auto& task : tasks) {
						m_taskList->AddTask(task);
					}
				}

				void ToBallRunTask::InitializeParametor() {
					constexpr float MoveSpeed = 8.5f;
					constexpr float NearTargetRange = 1.5f;

					//Astar�ŖڕW�̋߂��܂ňړ�����p�����[�^
					m_param.moveAstarParam->movePositionsParam->moveParamPtr->speed = MoveSpeed;
					m_param.moveAstarParam->movePositionsParam->moveParamPtr->targetNearRange = 1.5f;
					m_param.moveAstarParam->movePositionsParam->moveParamPtr->moveType = basecross::Task::ToTargetMove_MoveType::SeekVelocity;

					//�ړI�n�̋߂��܂ňړ������ꍇ�̐ݒ�
					m_param.targetSeekParam->toTargetMoveParam->speed = MoveSpeed;
					m_param.targetSeekParam->toTargetMoveParam->moveType = basecross::Task::ToTargetMove::MoveType::SeekVelocity;
					m_param.targetSeekParam->toTargetMoveParam->targetNearRange = NearTargetRange;
				}

				void ToBallRunTask::Rotation() {
					auto rotationController = m_rotationController.lock();
					auto velocityManager = m_velocityManager.lock();
					if (!velocityManager || !rotationController) {
						return;
					}

					rotationController->SetDirection(velocityManager->GetVelocity());
				}

				bool ToBallRunTask::IsEnd() const { return m_taskList->IsEnd(); }

			}
		}
	}
}