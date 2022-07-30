/*!
@file VernierEffect.h
@brief VernierEffect�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "EffectBase.h"

namespace basecross {

	namespace Effect {

		//--------------------------------------------------------------------------------------
		/// �o�[�j�A�G�t�F�N�g
		//--------------------------------------------------------------------------------------
		class VernierEffect : public Effect::Base
		{
		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			VernierEffect(const std::shared_ptr<GameObject>& objPtr);

			void OnCreate() override;

		};

	}
}