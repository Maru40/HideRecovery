/*!
@file EvadeSlimeObject.h
@brief EvadeSlimeObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	namespace Enemy {

		namespace EvadeSlime {

			//--------------------------------------------------------------------------------------
			/// 逃げるスライムオブジェクト
			//--------------------------------------------------------------------------------------
			class EvadeSlimeObject : public SlimeObjectBase
			{
			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="stage">このクラスが所属するステージ</param>
				EvadeSlimeObject(const std::shared_ptr<Stage>& stage);

				void OnCreate() override;

			private:

				void CreateModel() override;
			};

		}
	}
}