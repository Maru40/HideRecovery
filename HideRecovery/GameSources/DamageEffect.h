/*!
@file DamageEffect.h
@brief DamageEffectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class BoardObject;

	namespace Builder {
		struct VertexPCTParametor;
	}

	namespace Effect {

		//--------------------------------------------------------------------------------------
		/// ダメージエフェクト
		//--------------------------------------------------------------------------------------
		class Damage : public Effect::Base
		{
		private:

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			Damage(const std::shared_ptr<GameObject>& objPtr);

			virtual void OnCreate() override;

		private:
			/// <summary>
			/// 横長の板ポリゴンを生成する生成データの取得
			/// </summary>
			/// <returns>横長の板ポリゴンの生成データ</returns>
			ParticleData CreateLineVData();

			/// <summary>
			/// 点板ポリゴンの生成データの取得
			/// </summary>
			/// <returns>点板ポリゴンの生成データ</returns>
			ParticleData CretaePointData();

		};

	}
}