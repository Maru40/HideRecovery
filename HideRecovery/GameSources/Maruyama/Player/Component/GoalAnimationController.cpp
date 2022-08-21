
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

#include "Watanabe/Component/PlayerAnimator.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �S�[�����̃A�j���[�V�����R���g���[���[�̃p�����[�^
	//--------------------------------------------------------------------------------------

	GoalAnimationController_Parametor::GoalAnimationController_Parametor() :
		startPosition(Vec3(0)),
		dunkPosition(Vec3(0)),
		jumpPower(Vec3(0.0f, 1.0f, 0.0f)),
		preliminaryJumpParam(std::make_shared<Task::Wait::Parametor>(0.0f))
	{}

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

		//�S�[�����̃A�j���[�V�����C�x���g��o�^
		animator->AddAnimationEvent(
			PlayerAnimationState::State::Goal1,
			[&]() { StartAnimationEvent(); },
			[&]() { return UpdateAnimationEvent(); },
			[&]() { ExitAnimationEvent(); }
		);
	}

	void GoalAnimationController::StartAnimationEvent() {
		m_param.startPosition = transform->GetPosition();

		SelectTask();
	}

	bool GoalAnimationController::UpdateAnimationEvent() {
		m_taskList->UpdateTask();
		return m_taskList->IsEnd();
	}

	void GoalAnimationController::ExitAnimationEvent() {
		m_taskList->ForceStop();
	}

	void GoalAnimationController::DefineTask() {
		//�W�����v�̗���
		m_taskList->DefineTask(TaskEnum::PreliminaryJump, std::make_shared<Task::Wait>(m_param.preliminaryJumpParam));

		//�_���N�ʒu�܂ňړ�
		//m_taskList->DefineTask(TaskEnum::DunkMove, std::make_shared<Task_ToTargetMove>());

		//�_���N��̑ҋ@

		//���̈ʒu�ɖ߂�ړ�

		//�I�����ҋ@
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