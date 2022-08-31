
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
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class GameTimer;

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
	struct AnimationParametor {
		using State = PlayerAnimationCtrl_State;

		State stateType;
		wstring stateName;
		int startTime;
		int timeLength;
		bool loop;
		float timeParSecond;
		float updateSpeed;		//�X�V�X�s�[�h

		std::function<void()> startEvent;
		std::function<bool(const float)> updateEvent;
		std::function<void()> exitEvent;

		AnimationParametor();

		AnimationParametor(
			const State& state,
			const wstring& name,
			const int startTime,
			const int endTime,
			const bool loop,
			const float updateSpeed,
			const std::shared_ptr<I_AnimationStateNode>& stateNode
		);

		AnimationParametor(
			const State& state,
			const wstring& name,
			const int startTime,
			const int endTime,
			const bool loop,
			const float updateSpeed,
			const function<void()>& startEvent,
			const function<bool(const float)>& updateEvent,
			const function<void()>& exitEvent
		);

		AnimationParametor(
			const State& state,
			const wstring& name,
			const int startTime,
			const int endTime,
			const bool loop,
			const float parSecond,
			const float updateSpeed,
			const function<void()>& startEvent,
			const function<bool(const float)>& updateEvent,
			const function<void()>& exitEvent
		);
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
		unordered_map<State, AnimationParametor> m_parametorMap;	//�p�����[�^�}�b�v

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
		bool Idle(const float speed);

		/// <summary>
		/// ����
		/// </summary>
		bool Walk(const float speed);

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


	//--------------------------------------------------------------------------------------
	/// �A�j���[�V�����X�e�[�g�m�[�h
	//--------------------------------------------------------------------------------------

	namespace AnimationStateNode {

		//--------------------------------------------------------------------------------------
		/// ���ɃA�C�e����u���A�j���[�V����
		//--------------------------------------------------------------------------------------
		class PutItem_Floor : public AnimationStateNodeBase
		{
		public:
			PutItem_Floor(const std::shared_ptr<GameObject>& objPtr);

			virtual ~PutItem_Floor() = default;

			void OnStart() override;
			bool OnUpdate(const float speed) override;
			void OnExit() override;
		};

		//--------------------------------------------------------------------------------------
		/// �B���I�u�W�F�N�g�ɃA�C�e����u���A�j���[�V����
		//--------------------------------------------------------------------------------------
		class PutItem_HideObject : public AnimationStateNodeBase
		{
		public:
			PutItem_HideObject(const std::shared_ptr<GameObject>& objPtr);

			virtual ~PutItem_HideObject() = default;

			void OnStart() override;
			bool OnUpdate(const float speed) override;
			void OnExit() override;
		};
	}

}

//endbasecross