/*!
@file GoalAnimationController.h
@brief GoalAnimationController�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	template<class T>
	class TaskList;
	class Goal;

	namespace Enemy {
		class AIPlayerStator;
	}

	namespace Task {
		struct Wait_Parametor;
		struct ToTargetMove_Parametor;
		struct ReturnJump_Parametor;
	}

	//--------------------------------------------------------------------------------------
	/// �S�[�����̃A�j���[�V�����R���g���[���[�̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct GoalAnimationController_Parametor
	{
		Vec3 startPosition;		//�J�n�ʒu
		Vec3 dunkPosition;		//�_���N����ʒu
		Vec3 dunkBallPosition;	//�{�[�����_���N����ʒu
		float jumpDegree;		//�W�����v����p�x
		float dunkMoveSpeed;	//�_���N�ɂ����鎞��

		std::shared_ptr<Task::Wait_Parametor> preliminaryJumpParam;		//�W�����v���߃^�X�N�p�����[�^
		std::shared_ptr<Task::ToTargetMove_Parametor> dunkMoveParam;	//�_���N����Ƃ���܂ł̈ړ�
		std::shared_ptr<Task::Wait_Parametor> dunkAfterWaitParam;		//�_���N��̑ҋ@
		std::shared_ptr<Task::ReturnJump_Parametor> returnJumpParam;	//�_���N��̌��̈ʒu�ɖ߂鏈��
		std::shared_ptr<Task::Wait_Parametor> endWaitParam;				//���n��̍d��

		GoalAnimationController_Parametor();

		float GetJumpRad() const;
	};

	//--------------------------------------------------------------------------------------
	/// �S�[�����̃A�j���[�V�����R���g���[���[
	//--------------------------------------------------------------------------------------
	class GoalAnimationController : public Component
	{
	public:
		using Parametor = GoalAnimationController_Parametor;

		enum class TaskEnum {
			PreliminaryJump,	//�W�����v�̗���
			DunkMove,			//�_���N����Ƃ���܂ňړ�
			DunkWait,			//�_���N��̑ҋ@
			ReturnMove,			//�_���N��̌��̈ʒu�ɖ߂鏈��
			EndWait,			//���n��̍d��
		};

	private:
		Parametor m_param;		//�p�����[�^

		std::weak_ptr<Goal> m_goal;						//�S�[��
		std::weak_ptr<GameObject> m_ball;				//�{�[��
		std::unique_ptr<TaskList<TaskEnum>> m_taskList;	//�^�X�N�Ǘ�
		std::weak_ptr<GameObject> m_goalPerson;			// �S�[�������l
		std::weak_ptr<Enemy::AIPlayerStator> m_stator;	//�X�e�[�^�[

		SoundClip m_goalSoundClip;
		SoundClip m_goalStartSE;

	public:
		GoalAnimationController(const std::shared_ptr<GameObject>& objPtr);

		void OnLateStart() override;
		void OnUpdate() override;

		void OnDisable() override;

	private:
		/// <summary>
		/// �J�n�A�j���[�V�����C�x���g
		/// </summary>
		void StartAnimationEvent();

		/// <summary>
		/// �X�V���A�j���[�V�����C�x���g
		/// </summary>
		/// <returns>�A�j���[�V�������I�����Ă��邩�ǂ���</returns>
		bool UpdateAnimationEvent();

		/// <summary>
		/// �I�����A�j���[�V�����C�x���g
		/// </summary>
		void ExitAnimationEvent();

		/// <summary>
		/// �^�X�N�̒�`
		/// </summary>
		void DefineTask();

		/// <summary>
		/// �^�X�N�̑I��
		/// </summary>
		void SelectTask();

	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �_���N�ʒu�̐ݒ�
		/// </summary>
		/// <param name="position">�_���N�ʒu</param>
		void SetDunkPosition(const Vec3& position) noexcept { m_param.dunkPosition = position; }

		/// <summary>
		/// �_���N�ʒu�̎擾
		/// </summary>
		/// <returns>�_���N�ʒu</returns>
		Vec3 GetDunkPosition() const noexcept { return m_param.dunkPosition; }

		void SetDunkBallPosition(const Vec3& position) noexcept { m_param.dunkBallPosition = position; }

		Vec3 GetDunkBallPosition() const noexcept { return m_param.dunkBallPosition; }

		void SetGoal(const std::shared_ptr<Goal>& goal) { m_goal = goal; }

		std::shared_ptr<Goal> GetGoal() const { return m_goal.lock(); }

		void SetGoalPerson(const std::shared_ptr<GameObject>& obj) { m_goalPerson = obj; }

		/// <summary>
		/// �S�[���A�j���[�V�����Đ���
		/// </summary>
		bool IsGoalAnimation() const;

		/// <summary>
		/// �������Z�b�g
		/// </summary>
		void ForceReset();

		void StartGoalAnimation();
	};
}