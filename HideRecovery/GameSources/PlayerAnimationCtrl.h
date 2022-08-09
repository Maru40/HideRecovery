
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
	struct AddAnimeParam {
		using State = PlayerAnimationCtrl_State;

		State stateType;
		wstring stateName;
		int startTime;
		int timeLength;
		bool loop;
		float timeParSecond;
		float updateSpeed;		//�X�V�X�s�[�h

		function<void(T&)> func;

		AddAnimeParam(
			const State& state,
			const wstring& name,
			const int startTime,
			const int endTime,
			const bool loop,
			const function<void(T&)> func
		) :
			AddAnimeParam(state, name, startTime, endTime, loop, 1.0f, func)
		{}

		AddAnimeParam(
			const State& state,
			const wstring& name,
			const int startTime,
			const int endTime,
			const bool loop,
			const float parSecond,
			const function<void(T&)> func
		) :
			stateType(state),
			stateName(name),
			startTime(startTime),
			loop(loop),
			timeParSecond(parSecond),
			func(func)
		{
			timeLength = endTime - startTime;
		}
	};

	//--------------------------------------------------------------------------------------
	/// �A�j���[�V�����p�����[�^
	//--------------------------------------------------------------------------------------
	struct AnimationParametor {
		wstring stateName;	//�X�e�[�g�o�^�Ɏg�p�������O
		float updateSpeed;	//�X�V�X�s�[�h
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
		unordered_map<State, wstring> m_stateStringMap;									//State��wstring�œo�^����}�b�v�B
		unordered_map<State, std::function<void(PlayerAnimationCtrl&)>> m_animations;	//�A�j���[�V�����̍X�V�C�x���g�}�b�v

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

		/// <summary>
		/// �A�j���[�V�����̐ݒ�
		/// </summary>
		/// <returns>�A�j���[�V����</returns>
		void SetAnimaiton(const State& state, const function<void(PlayerAnimationCtrl&)> func);

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