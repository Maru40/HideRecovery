
/*!
@file PlayerDeader.h
@brief PlayerDeader�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class PlayerAnimator;

	//--------------------------------------------------------------------------------------
	/// �v���C���[�̎��S�Ǘ�����N���X�̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct PlayerDeader_Parametor {
		
	};

	//--------------------------------------------------------------------------------------
	/// �v���C���[�̎��S�Ǘ�����N���X
	//--------------------------------------------------------------------------------------
	class PlayerDeader : public Component
	{
	public:
		using Parametor = PlayerDeader_Parametor;

	private:
		std::weak_ptr<PlayerAnimator> m_animator;	//�A�j���[�^�[
		std::function<void()> m_updateFunction;		//�X�V����

	public:
		PlayerDeader(const std::shared_ptr<GameObject>& objPtr);

		void OnLateStart() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// �A�j���[�V�����Ď���
		/// </summary>
		void ObserveAnimation();

	public:
		/// <summary>
		/// ���S�J�n
		/// </summary>
		void StartDead();

		/// <summary>
		/// ���S�ł����Ԃ��ǂ���
		/// </summary>
		/// <returns>���S�ł���Ȃ�true</returns>
		bool IsDead();
	};

}