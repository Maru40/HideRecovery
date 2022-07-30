/*!
@file BigNormalSlime.h
@brief BigNormalSlimeなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	struct KnockBackData;

	namespace Enemy {

		class EnemyBase;
		class SlimeBase;

		namespace BigSlime {

			//--------------------------------------------------------------------------------------
			/// 巨大スライムのクラス
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
				/// パラメータ
				//--------------------------------------------------------------------------------------

				virtual void KnockBack(const KnockBackData& data) override;

				virtual PressedEnergy Pressed(const PressData& pressData) override;
			};

		}
	}
}