/*!
@file BigSlimeBase.h
@brief BigSlimeBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		namespace BigSlime {

			//--------------------------------------------------------------------------------------
			///	����X���C���̋K��N���X
			//--------------------------------------------------------------------------------------
			class BigSlimeBase : public SlimeBase
			{
			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
				BigSlimeBase(const std::shared_ptr<GameObject>& objPtr);

				//--------------------------------------------------------------------------------------
				///	�C���^�[�t�F�[�X�̎���
				//--------------------------------------------------------------------------------------
				
				virtual PressedEnergy Pressed(const PressData& pressData) override;

				/// <summary>
				/// �������݃I�u�W�F�N�g�Ɋ������܂ꂽ���̏���
				/// </summary>
				void InvolvedPressed();

				virtual void Spread(const PressData& pressData) override;

				virtual Motion::KnockBack_Parametor GetPressedMotionData(const PressData& pressData) override;
			};

		}
	}
}