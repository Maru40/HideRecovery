/*!
@file VernierEffect.h
@brief VernierEffectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "EffectBase.h"

namespace basecross {

	namespace Effect {

		//--------------------------------------------------------------------------------------
		/// バーニアエフェクト
		//--------------------------------------------------------------------------------------
		class VernierEffect : public Effect::Base
		{
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			VernierEffect(const std::shared_ptr<GameObject>& objPtr);

			void OnCreate() override;

		};

	}
}