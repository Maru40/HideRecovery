
/*!
@file ShootSlime.h
@brief ShootSlime�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	struct KnockBackData;

	namespace Enemy {

		class EnemyBase;
		class SlimeBase;

		namespace ShootSlime {

			//--------------------------------------------------------------------------------------
			/// �V���b�g�X���C��
			//--------------------------------------------------------------------------------------
			class ShootSlime : public SlimeBase, public I_KnockBack
			{
			public:
				/// <summary>
				/// �V���b�g�X���C��
				/// </summary>
				/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
				ShootSlime(const std::shared_ptr<GameObject>& objPtr);

				//--------------------------------------------------------------------------------------
				/// �C���^�[�t�F�[�X�̎���
				//--------------------------------------------------------------------------------------

				virtual void KnockBack(const KnockBackData& data) override;

				virtual PressedEnergy Pressed(const PressData& pressData) override;

			};

		}
	}
}