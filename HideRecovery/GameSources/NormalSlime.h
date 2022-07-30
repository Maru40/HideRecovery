
/*!
@file NormalSlime.h
@brief NormalSlimeなど
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

		namespace NormalSlime {

			//--------------------------------------------------------------------------------------
			/// 通常スライムのステーター
			//--------------------------------------------------------------------------------------
			class NormalSlime : public SlimeBase, public I_KnockBack
			{
			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
				NormalSlime(const std::shared_ptr<GameObject>& objPtr);

				//--------------------------------------------------------------------------------------
				/// インターフェースの実装
				//--------------------------------------------------------------------------------------

				virtual void KnockBack(const KnockBackData& data) override;

				virtual PressedEnergy Pressed(const PressData& pressData) override;

			};

		}
	}
}