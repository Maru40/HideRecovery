/*!
@file BigShootSlimeObject.h
@brief BigShootSlimeObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	namespace Enemy {
		namespace BigShootSlime {

			//--------------------------------------------------------------------------------------
			/// 巨大シュートスライムのオブジェクト
			//--------------------------------------------------------------------------------------
			class BigShootSlimeObject : public SlimeObjectBase
			{
			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="stage">このクラスを所有するステージ</param>
				BigShootSlimeObject(const std::shared_ptr<Stage>& stage);

				void OnCreate() override;

			private:
				virtual void CreateModel() override;
				virtual void SettingScalePop() override;

			};

		}
	}
}