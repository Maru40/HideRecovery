/*!
@file TackleEffect.h
@brief TackleEffect�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class BoardObject;

	namespace Effect {

		//--------------------------------------------------------------------------------------
		/// �^�b�N���G�t�F�N�g
		//--------------------------------------------------------------------------------------
		class Tackle : public Effect::Base
		{
		public:
			Tackle(const std::shared_ptr<GameObject>& objPtr);

			virtual void OnCreate() override;

		private:
			/// <summary>
			/// �f�t�H���g�I�[���̐����f�[�^���擾
			/// </summary>
			/// <returns>�f�t�H���g�I�[���̐����f�[�^</returns>
			CreateAuraData CreateDefaultAura();

			/// <summary>
			/// �ړ��I�[���̐����f�[�^���擾
			/// </summary>
			/// <returns>�ړ��I�[���̐����f�[�^</returns>
			CreateAuraData CreateMoveAura();

		};

	}
}