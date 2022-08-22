
/*!
@file GoalAnimationController.cpp
@brief GoalAnimationController�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "GoalAnimationController.h"

#include "TaskList.h"

#include "TimeHelper.h"
#include "GameTimer.h"
#include "Task_Wait.h"

#include "Task_ToTargetMove.h"

#include "Task_GoalAnimation.h"

#include "Watanabe/Component/PlayerAnimator.h"

#include "VelocityManager.h"
#include "RotationController.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �S�[�����̃A�j���[�V�����R���g���[���[�̃p�����[�^
	//--------------------------------------------------------------------------------------

	GoalAnimationController_Parametor::GoalAnimationController_Parametor() :
		startPosition(Vec3(0)),
		dunkPosition(Vec3(0)),
		jumpDegree(30.0f),
		dunkMoveSpeed(3.0f),

		preliminaryJumpParam(std::make_shared<Task::Wait::Parametor>(0.0f)),
		dunkMoveParam(std::make_shared<Task::ToTargetMove::Parametor>(3.0f, 0.1f, Task::ToTargetMove_MoveType::Lerp)),
		dunkAfterWaitParam(std::make_shared<Task::Wait::Parametor>(0.5f)),
		returnJumpParam(std::make_shared<Task::ReturnJump_Parametor>()),
		endWaitParam(std::make_shared<Task::Wait::Parametor>(0.5f))
	{}

	float GoalAnimationController_Parametor::GetJumpRad() const { return XMConvertToRadians(jumpDegree); }

	//--------------------------------------------------------------------------------------
	/// �S�[�����̃A�j���[�V�����R���g���[���[�{��
	//--------------------------------------------------------------------------------------

	GoalAnimationController::GoalAnimationController(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr),
		m_taskList(new TaskList<TaskEnum>())
	{}

	void GoalAnimationController::OnLateStart() {
		auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false);
		if (!animator) {	//animator���Ȃ������珈�����I��
			return;
		}

		DefineTask();

		//�S�[�����̃A�j���[�V�����C�x���g��o�^
		animator->AddAnimationEvent(
			PlayerAnimationState::State::Goal1,
			[&]() { StartAnimationEvent(); },
			[&]() { return UpdateAnimationEvent(); },
			[&]() { ExitAnimationEvent(); }
		);
	}

	void GoalAnimationController::StartAnimationEvent() {
		m_param.startPosition = transform->GetPosition();	//�J�n�ʒu�̐ݒ�

		m_param.dunkMoveParam->startPosition = m_param.startPosition;
		m_param.dunkMoveParam->endPosition = m_param.dunkPosition;

		m_param.returnJumpParam->returnDirect = transform->GetPosition() - m_param.dunkPosition;

		if (auto gravity = GetGameObject()->GetComponent<Gravity>(false)) {	//�d�͂̉���
			gravity->SetUpdateActive(false);
		}

		if (auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false)) {	//���x��~
			velocityManager->ResetAll();
		}

		if (auto rotationController = GetGameObject()->GetComponent<RotationController>(false)) {
			rotationController->SetDirect(m_param.dunkPosition - transform->GetPosition());
		}

		m_taskList->ForceStop();
		SelectTask();
	}

	bool GoalAnimationController::UpdateAnimationEvent() {
		m_taskList->UpdateTask();
		return m_taskList->IsEnd();
	}

	void GoalAnimationController::ExitAnimationEvent() {
		if (auto gravity = GetGameObject()->GetComponent<Gravity>(false)) {	//�d�͂̉���
			gravity->SetUpdateActive(true);
		}
	}

	void GoalAnimationController::DefineTask() {
		auto object = GetGameObject();

		//�W�����v�̗���
		m_param.preliminaryJumpParam->start = [&]() { //�J�n���C�x���g

		};	
		m_taskList->DefineTask(TaskEnum::PreliminaryJump, std::make_shared<Task::Wait>(m_param.preliminaryJumpParam));

		//�_���N�ʒu�܂ňړ�
		auto& dunkMoveParam = m_param.dunkMoveParam;
		m_taskList->DefineTask(TaskEnum::DunkMove, std::make_shared<Task::ToTargetMove>(object, m_param.dunkMoveParam));

		//�_���N��̑ҋ@
		m_param.dunkAfterWaitParam->exit = [&]() {
			if (auto gravity = GetGameObject()->GetComponent<Gravity>(false)) {	//�d�͂̉���
				//gravity->SetUpdateActive(true);
			}
		};
		m_taskList->DefineTask(TaskEnum::DunkWait, std::make_shared<Task::Wait>(m_param.dunkAfterWaitParam));

		//���̈ʒu�ɖ߂�ړ�
		m_taskList->DefineTask(TaskEnum::ReturnMove, std::make_shared<Task::ReturnJump>(GetGameObject(), m_param.returnJumpParam));

		//�I�����ҋ@
		m_param.endWaitParam->exit = [&]() {
			if (auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false)) {
				animator->ChangePlayerAnimation(PlayerAnimationState::State::Wait);
			}
		};
		m_taskList->DefineTask(TaskEnum::EndWait, std::make_shared<Task::Wait>(m_param.endWaitParam));
	}

	void GoalAnimationController::SelectTask() {
		TaskEnum tasks[] = {
			TaskEnum::PreliminaryJump,
			TaskEnum::DunkMove,
			TaskEnum::DunkWait,
			TaskEnum::ReturnMove,
			TaskEnum::EndWait
		};

		for (auto& task : tasks) {
			m_taskList->AddTask(task);
		}
	}
}