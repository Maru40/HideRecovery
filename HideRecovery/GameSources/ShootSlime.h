
/*!
@file ShootSlime.h
@brief ShootSlimeなど
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

		namespace ShootSlime {

			//--------------------------------------------------------------------------------------
			/// ショットスライム
			//--------------------------------------------------------------------------------------
			class ShootSlime : public SlimeBase, public I_KnockBack
			{
			public:
				/// <summary>
				/// ショットスライム
				/// </summary>
				/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
				ShootSlime(const std::shared_ptr<GameObject>& objPtr);

				//--------------------------------------------------------------------------------------
				/// インターフェースの実装
				//--------------------------------------------------------------------------------------

				virtual void KnockBack(const KnockBackData& data) override;

				virtual PressedEnergy Pressed(const PressData& pressData) override;

			};

		}
	}
}