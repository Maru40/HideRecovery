
/*!
@file CombatCalculater.cpp
@brief CombatCalculaterのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "HidePlacePatrolCalculater.h"

#include "Watanabe/Component/PlayerStatus.h"
#include "Maruyama/Utility/Component/Targeted.h"

#include "Maruyama/Utility/Mathf.h"

namespace basecross {

	namespace Enemy {

		namespace Calculater {

			float HidePlacePatorl::CalculateEvalutionValue(
				const std::shared_ptr<GameObject>& selfObject,
				const std::shared_ptr<GameObject>& target
			) {
				float result = 0.0f;

				float toTargetRange = maru::Utility::CalcuToTargetVec(selfObject, target).length();
				result += toTargetRange;	//距離を評価値とする。

				//ターゲット本体の優先度を追加
				if (auto targeted = target->GetComponent<Targeted>(false)) {
					result *= targeted->GetPriority();
				}

				//相手のhpによって優先度をあげる。
				if (auto status = target->GetComponent<PlayerStatus>(false)) {
					float hpRate = (float)status->GetStatus().hp / (float)status->GetStatus().maxHp;
					result *= hpRate;	//hpが低いほど、距離を減衰(ターゲットにしやすくする。)
				}

				return result;
			}

		}
	}
}