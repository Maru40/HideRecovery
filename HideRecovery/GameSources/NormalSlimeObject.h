/*!
@file NormalSlimeObject.h
@brief NormalSlimeObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class SlimeObjectBase;

		//--------------------------------------------------------------------------------------
		/// 通常スライムオブジェクト
		//--------------------------------------------------------------------------------------
		class NormalSlimeObject : public SlimeObjectBase
		{
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="stage">このクラスが所属するステージ</param>
			NormalSlimeObject(const std::shared_ptr<Stage>& stage);

			void OnCreate() override;

		private:
			virtual void CreateModel();

			
		};
	}
}