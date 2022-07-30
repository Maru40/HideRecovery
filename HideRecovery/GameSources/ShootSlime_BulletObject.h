/*!
@file ShootSlime_BulletObject.h
@brief ShootSlime_BulletObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		namespace ShootSlime {

			//--------------------------------------------------------------------------------------
			/// ショットスライムの弾オブジェクト
			//--------------------------------------------------------------------------------------
			class BulletObject : public GameObject
			{
			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="stage">このクラスが所属するステージ</param>
				BulletObject(const std::shared_ptr<Stage>& stage);

				void OnCreate() override;

			private:
				/// <summary>
				/// 描画設定
				/// </summary>
				void SettingDraw();

			};

		}
	}
}