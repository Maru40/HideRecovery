/*!
@file BigSlimeObject.h
@brief BigSlimeObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	namespace Enemy {
		namespace BigSlime{

			//--------------------------------------------------------------------------------------
			/// 巨大スライムオブジェクト
			//--------------------------------------------------------------------------------------
			class BigSlimeObject : public SlimeObjectBase
			{
			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="stage">このクラスを所有するステージ</param>
				BigSlimeObject(const std::shared_ptr<Stage>& stage);

				void OnCreate() override;

			private:
				virtual void CreateModel() override;
				virtual void SettingScalePop() override;

			};
		
		}
	}
}