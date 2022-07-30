/*!
@file ShootSlimeObject.h
@brief ShootSlimeObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	namespace Enemy
	{
		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class SlimeObjectBase;

		namespace ShootSlime {

			//--------------------------------------------------------------------------------------
			/// ショットスライムオブジェクト
			//--------------------------------------------------------------------------------------
			class ShootSlimeObject : public SlimeObjectBase
			{
			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="stage">このクラスが所属するステージ</param>
				ShootSlimeObject(const std::shared_ptr<Stage>& stage);

				void OnCreate() override;

			protected:
				virtual void CreateModel() override;
			};
		}
	}
}