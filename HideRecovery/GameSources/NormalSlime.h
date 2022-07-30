
/*!
@file NormalSlime.h
@brief NormalSlime�Ȃ�
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

		namespace NormalSlime {

			//--------------------------------------------------------------------------------------
			/// �ʏ�X���C���̃X�e�[�^�[
			//--------------------------------------------------------------------------------------
			class NormalSlime : public SlimeBase, public I_KnockBack
			{
			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
				NormalSlime(const std::shared_ptr<GameObject>& objPtr);

				//--------------------------------------------------------------------------------------
				/// �C���^�[�t�F�[�X�̎���
				//--------------------------------------------------------------------------------------

				virtual void KnockBack(const KnockBackData& data) override;

				virtual PressedEnergy Pressed(const PressData& pressData) override;

			};

		}
	}
}