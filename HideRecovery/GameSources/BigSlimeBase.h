/*!
@file BigSlimeBase.h
@brief BigSlimeBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		namespace BigSlime {

			//--------------------------------------------------------------------------------------
			///	巨大スライムの規定クラス
			//--------------------------------------------------------------------------------------
			class BigSlimeBase : public SlimeBase
			{
			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
				BigSlimeBase(const std::shared_ptr<GameObject>& objPtr);

				//--------------------------------------------------------------------------------------
				///	インターフェースの実装
				//--------------------------------------------------------------------------------------
				
				virtual PressedEnergy Pressed(const PressData& pressData) override;

				/// <summary>
				/// 巻き込みオブジェクトに巻き込まれた時の処理
				/// </summary>
				void InvolvedPressed();

				virtual void Spread(const PressData& pressData) override;

				virtual Motion::KnockBack_Parametor GetPressedMotionData(const PressData& pressData) override;
			};

		}
	}
}