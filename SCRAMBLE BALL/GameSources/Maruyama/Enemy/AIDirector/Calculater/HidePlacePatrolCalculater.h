
/*!
@file HidePlacePatorl.h
@brief HidePlacePatorlなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		namespace Calculater {

			class HidePlacePatorl {

			public:
				//ターゲットの評価値を計算して返す。
				static float CalculateEvalutionValue(
					const std::shared_ptr<GameObject>& selfObject,
					const std::shared_ptr<GameObject>& target
				);

			};
		}
	}
}