/*!
@file TackleEffect.h
@brief TackleEffectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class BoardObject;

	namespace Effect {

		//--------------------------------------------------------------------------------------
		/// タックルエフェクト
		//--------------------------------------------------------------------------------------
		class Tackle : public Effect::Base
		{
		public:
			Tackle(const std::shared_ptr<GameObject>& objPtr);

			virtual void OnCreate() override;

		private:
			/// <summary>
			/// デフォルトオーラの生成データを取得
			/// </summary>
			/// <returns>デフォルトオーラの生成データ</returns>
			CreateAuraData CreateDefaultAura();

			/// <summary>
			/// 移動オーラの生成データを取得
			/// </summary>
			/// <returns>移動オーラの生成データ</returns>
			CreateAuraData CreateMoveAura();

		};

	}
}