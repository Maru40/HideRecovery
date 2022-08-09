
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
		Idle,				//�ҋ@
		Walk,				//����
		PutItem_Floor,		//���ɒu��
		PutItem_HideObject,	//�B���I�u�W�F�N�g�ɒu��
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

		function<void(T&)> startEvent;
		function<void(T&)> updateEvent;
		function<void(T&)> exitEvent;

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
			AnimationParametor(state, name, startTime, endTime, loop, 30.0f, updateSpeed, nullptr, updateEvent, nullptr)
		{}

		AnimationParametor(
			const State& state,
			const wstring& name,
			const int startTime,
			const int endTime,
			const bool loop,
			const float parSecond,
			const float updateSpeed,
			const function<void(T&)> startEvent,
			const function<void(T&)> updateEvent,
			const function<void(T&)> exitEvent
		) :
			stateType(state),
			stateName(name),
			startTime(startTime),
			loop(loop),
			timeParSecond(parSecond),
			updateSpeed(updateSpeed),
			startEvent(startEvent),
			updateEvent(updateEvent),
			exitEvent(exitEvent)
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
		/// �A�j���[�V�����C�x���g
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
		/// ���ɒu��
		/// </summary>
		void PutItem_Floor();

		/// <summary>
		/// �B���I�u�W�F�N�g�ɓ����
		/// </summary>
		void PutItem_HideObject();

		/// <summary>
		/// �f�t�H���g�Đ��֐�
		/// </summary>
		void DefaultPlay(const bool isEndTransitionIdle = true);

	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

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
		State GetCurrentAnimaiton() const noexcept { return m_currentState; }

		/// <summary>
		/// ���݂̃A�j���[�V�����X�V���x���擾
		/// </summary>
		float GetCurrentUpdateSpeed() const { return m_parametorMap.at(m_currentState).updateSpeed; }
	};

}

//endbasecross