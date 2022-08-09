
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
		Wait,
		Walk,
		Put_Floor,
		Put_HideObject,
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
	/// �v���C���[�̃A�j���[�V�����Ǘ�
	//--------------------------------------------------------------------------------------
	class PlayerAnimationCtrl : public AnimationCtrl
	{
	public:
		using State = PlayerAnimationCtrl_State;

	private:
		unordered_map<State, wstring> m_stateStringMap;
		unordered_map<wstring, std::function<void(PlayerAnimationCtrl&)>> m_animations;

		State m_currentState;

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		PlayerAnimationCtrl(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		//--------------------------------------------------------------------------------------
		/// �Đ��������A�j���[�V����
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �ҋ@
		/// </summary>
		void Wait();

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
		/// �A�j���[�V�����̐ݒ�
		/// </summary>
		/// <returns>�A�j���[�V����</returns>
		void SetAnimaiton(const wstring& animeName, const function<void(PlayerAnimationCtrl&)> func);

		/// <summary>
		/// �A�j���[�V�����̐؂�ւ�
		/// </summary>
		void ChangeAnimation(const wstring& animeType);

		/// <summary>
		/// ���݂̃A�j���[�V�����X�e�[�g���擾
		/// </summary>
		wstring GetCurrentAnimaiton() const {
			auto draw = GetGameObject()->GetComponent<SmBaseDraw>();
			return draw->GetCurrentAnimation();
		}
	};

}

//endbasecross