/*!
@file BigShootSlime.h
@brief BigShootSlimeなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	struct KnockBackData;

	namespace Enemy {

		class EnemyBase;
		class SlimeBase;

		namespace BigShootSlime {

			//--------------------------------------------------------------------------------------
			///	巨大スライムのショットスライム
			//--------------------------------------------------------------------------------------
			class BigShootSlime : public BigSlime::BigSlimeBase, public I_KnockBack
			{
			private:

			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
				BigShootSlime(const std::shared_ptr<GameObject>& objPtr);

				//--------------------------------------------------------------------------------------
				///	インターフェースの実装
				//--------------------------------------------------------------------------------------

				virtual void KnockBack(const KnockBackData& data) override;

				virtual PressedEnergy Pressed(const PressData& pressData) override;
			};

		}
	}
}