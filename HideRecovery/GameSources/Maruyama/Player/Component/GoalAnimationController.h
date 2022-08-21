
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

	namespace Task {
		struct Wait_Parametor;
	}

	//--------------------------------------------------------------------------------------
	/// �S�[�����̃A�j���[�V�����R���g���[���[�̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct GoalAnimationController_Parametor
	{
		Vec3 startPosition;		//�J�n�ʒu
		Vec3 dunkPosition;		//�_���N����ʒu
		Vec3 jumpPower;			//�W�����v�����
		float dunkMoveSpeed;	//�_���N�ɂ����鎞��

		std::shared_ptr<Task::Wait_Parametor> preliminaryJumpParam;	//�W�����v���߃^�X�N�p�����[�^
		//�_���N����Ƃ���܂ł̈ړ�

		GoalAnimationController_Parametor();
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

		std::unique_ptr<TaskList<TaskEnum>> m_taskList;	//�^�X�N�Ǘ�

	public:
		GoalAnimationController(const std::shared_ptr<GameObject>& objPtr);

		void OnLateStart() override;

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
	};

}