/*!
@file Animator_Player.h
@brief Animator_Player�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Trigger.h"
#include "AnimationHelper.h"

namespace basecross
{

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	template<class StructMember, class StateEnum>
	class AnimatorMainStateMachine;

	template<class StructMember, class StateEnum>
	class Animator;

	template<class TaskEnum>
	class TaskList;

	struct Task_MovePositions_Parametor;

	//--------------------------------------------------------------------------------------
	/// �v���C���[�A�j���[�V�����̑J�ڏ��������o�[
	//--------------------------------------------------------------------------------------
	struct PlayerAnimator_TransitionMember
	{
		float walkSpeed = 0.0f;           //�ړ��X�s�[�h
		float transitionWalkSpeed = 3.0f; //�ړ��ɑJ�ڂ���X�s�[�h
		Trigger tackleTrigger;            
		Trigger aimTrigger;
	};

	//--------------------------------------------------------------------------------------
	/// �v���C���[�A�j���[�V�����̃X�e�[�g�^�C�v
	//--------------------------------------------------------------------------------------
	enum class PlayerAnimator_StateEnum {
		Idle,       //�ҋ@
		WalkStart,  //���s�J�n
		Walk,       //���s
		WalkEnd,    //���s�I��
		Tackle,     //�^�b�N��
		TackleEnd,  //�^�b�N����I��
		Aim,        //�G�C��
		Shot,       //������
		Damage,     //�_���[�W
		Death,      //���S
		WinIdle,    //�����ҋ@
		WinMove,    //�����ړ�
	};

	//--------------------------------------------------------------------------------------
	/// �v���C���[�̃A�j���[�V����
	//--------------------------------------------------------------------------------------
	class PlayerAnimator : public basecross::Animator<PlayerAnimator_TransitionMember, PlayerAnimator_StateEnum>
	{
	public:
		using TransitionMember = PlayerAnimator_TransitionMember;
		using StateEnum = PlayerAnimator_StateEnum;
		using StateMachine = basecross::AnimatorMainStateMachine<TransitionMember, StateEnum>;

	private:
		ex_weak_ptr<StateMachine> m_stateMachine;                         //�X�e�[�g�}�V��

		std::shared_ptr<Task_MovePositions_Parametor> m_winMoveParamPtr;  //�����ړ��p�����[�^
		 
		bool m_isPlayPlayerWalkSound;                                     //���s�T�E���h�𗘗p���邩�ǂ���

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		PlayerAnimator(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;

		/// <summary>
		/// �X�e�[�g�̕ύX
		/// </summary>
		/// <param name="stateEnum">�X�e�[�g�^�C�v</param>
		virtual void ChangeState(const StateEnum& stateEnum) override;

		/// <summary>
		/// �X�e�[�g�̋����ύX
		/// </summary>
		/// <param name="stateEnum">�X�e�[�g�^�C�v</param>
		void ForceChangeState(const StateEnum& stateEnum);

	private:
		/// <summary>
		/// �m�[�h�̐���
		/// </summary>
		void CreateNode();

		/// <summary>
		/// �ҋ@�m�[�h�̐���
		/// </summary>
		void CreateIdleNode();

		/// <summary>
		/// ���s�J�n�m�[�h�̐���
		/// </summary>
		void CreateWalkStartNode();

		/// <summary>
		/// ���s�m�[�h�̐���
		/// </summary>
		void CreateWalkNode();

		/// <summary>
		/// ���s�I���m�[�h�̐���
		/// </summary>
		void CreateWalkEndNode();

		/// <summary>
		/// �^�b�N���m�[�h�̐���
		/// </summary>
		void CreateTackleNode();

		/// <summary>
		/// �^�b�N���I���m�[�h�̐���
		/// </summary>
		void CreateTacleEndNode();

		/// <summary>
		/// �G�C���m�[�h�̐���
		/// </summary>
		void CreateAimNode();

		/// <summary>
		/// �V���b�g�m�[�h�̐���
		/// </summary>
		void CreateShotNode();

		/// <summary>
		/// �_���[�W�m�[�h�̐���
		/// </summary>
		void CreateDamageNode();

		/// <summary>
		/// ���S�m�[�h�̐���
		/// </summary>
		void CreateDeathNode();

		/// <summary>
		/// �����ҋ@�m�[�h�̐���
		/// </summary>
		void CreateWinIdleNode();

		/// <summary>
		/// �����ړ��̐���
		/// </summary>
		void CreateWinMoveNode();

		/// <summary>
		/// ���S�A�j���[�V�����I���C�x���g
		/// </summary>
		void EndDeathAnimationEvent();

	public:

	};

}