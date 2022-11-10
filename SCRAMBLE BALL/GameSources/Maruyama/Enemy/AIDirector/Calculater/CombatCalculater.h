
/*!
@file CombatCalculater.h
@brief CombatCalculaterなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		namespace Calculater {

			//--------------------------------------------------------------------------------------
			/// バトル中の計算系
			//--------------------------------------------------------------------------------------
			class Combat 
			{
			public:
				/// <summary>
				/// そのターゲットを狙う評価値を計算して返す。
				/// </summary>
				/// <param name="selfObject">自分自身のオブジェクト</param>
				/// <param name="target">狙うターゲット</param>
				/// <returns>ターゲットを狙う評価値</returns>
				static float CalculateEvalutionValue(
					const std::shared_ptr<GameObject>& selfObject,
					const std::shared_ptr<GameObject>& target
				);

				/// <summary>
				/// そのターゲットを狙う評価値を計算して返す。
				/// </summary>
				/// <param name="selfTransform">自分自身のトランスフォーム</param>
				/// <param name="target">狙うターゲット</param>
				/// <returns>ターゲットを狙う評価値</returns>
				static float CalculateEvalutionValue(
					const std::shared_ptr<Transform>& selfTransform,
					const std::shared_ptr<GameObject>& target
				);

			};

		}

	}
}