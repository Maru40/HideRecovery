
/*!
@file PlayerAnimationCtrl.h
@brief PlayerAnimationCtrl.h�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"
#include "AnimationCtrl.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �A�j���[�V�����̃X�e�[�g
	//--------------------------------------------------------------------------------------
	enum class PlayerAnimationCtrl_State {
		Idle,
		Walk,
		Put_Floor,
		Put_HideObject,
	};

	//--------------------------------------------------------------------------------------
	/// �J�ڃ����o�[
	//--------------------------------------------------------------------------------------
	struct PlayerAnimationCtrl_TransitionMember {
		float walkSpeed;

		PlayerAnimationCtrl_TransitionMember();
	};

	//--------------------------------------------------------------------------------------
	/// �A�j���[�V�����ǉ��p�����[�^
	//--------------------------------------------------------------------------------------
	template<class T>
	struct AnimationParametor {
		using State = PlayerAnimationCtrl_State;

		State stateType;
		wstring stateName;
		int startTime;
		int timeLength;
		bool loop;
		float timeParSecond;
		float updateSpeed;		//�X�V�X�s�[�h

		function<void(T&)> updateEvent;

		AnimationParametor():
			AnimationParametor(State(0), L"", 0, 0, false, 1.0f, nullptr)
		{}

		AnimationParametor(
			const State& state,
			const wstring& name,
			const int startTime,
			const int endTime,
			const bool loop,
			const float updateSpeed,
			const function<void(T&)> updateEvent
		) :
			AnimationParametor(state, name, startTime, endTime, loop, 1.0f, updateSpeed, updateEvent)
		{}

		AnimationParametor(
			const State& state,
			const wstring& name,
			const int startTime,
			const int endTime,
			const bool loop,
			const float parSecond,
			const float updateSpeed,
			const function<void(T&)> updateEvent
		) :
			stateType(state),
			stateName(name),
			startTime(startTime),
			loop(loop),
			timeParSecond(parSecond),
			updateSpeed(updateSpeed),
			updateEvent(updateEvent)
		{
			timeLength = endTime - startTime;
		}
	};

	//--------------------------------------------------------------------------------------
	/// �v���C���[�̃A�j���[�V�����Ǘ�
	//--------------------------------------------------------------------------------------
	class PlayerAnimationCtrl : public AnimationCtrl
	{
	public:
		using State = PlayerAnimationCtrl_State;
		using TransitionMember = PlayerAnimationCtrl_TransitionMember;

	private:
		unordered_map<State, AnimationParametor<PlayerAnimationCtrl>> m_parametorMap;	//�p�����[�^�}�b�v

		State m_currentState;						//���݂̃X�e�[�g
		TransitionMember m_transitionMember;		//�J�ڏ��������o�[

		std::weak_ptr<SmBaseDraw> m_drawComponent;	//�`��R���|�\�l���g

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		PlayerAnimationCtrl(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnStart() override;
		void OnUpdate() override;

	private:
		//--------------------------------------------------------------------------------------
		/// �Đ��������A�j���[�V����
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �ҋ@
		/// </summary>
		void Idle();

		/// <summary>
		/// ����
		/// </summary>
		void Walk();

		/// <summary>
		/// �f�t�H���g�Đ��֐�
		/// </summary>
		void DefaultPlay(const float speed = 15.0f);

	public:
		/// <summary>
		/// �A�j���[�V�����̐؂�ւ�
		/// </summary>
		void ChangeAnimation(const State& state);

		/// <summary>
		/// �����A�j���[�V�����؂�ւ�
		/// </summary>
		/// <param name="state">�؂�ւ���^�C�v</param>
		void ChangeForceAnimation(const State& state);

		/// <summary>
		/// ���݂̃A�j���[�V�����X�e�[�g���擾
		/// </summary>
		State GetCurrentAnimaiton() const noexcept { m_currentState; }
	};

}

//endbasecross