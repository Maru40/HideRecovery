/*!
@file BigNormalSlime.h
@brief BigNormalSlime�Ȃ�
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

		namespace BigSlime {

			//--------------------------------------------------------------------------------------
			/// ����X���C���̃N���X
			//--------------------------------------------------------------------------------------
			class BigNormalSlime : public BigSlime::BigSlimeBase, public I_KnockBack
			{
			private:

			public:
				/// <summary>
				/// 
				/// </summary>
				/// <param name="objPtr"></param>
				BigNormalSlime(const std::shared_ptr<GameObject>& objPtr);

				//--------------------------------------------------------------------------------------
				/// �p�����[�^
				//--------------------------------------------------------------------------------------

				virtual void KnockBack(const KnockBackData& data) override;

				virtual PressedEnergy Pressed(const PressData& pressData) override;
			};

		}
	}
}