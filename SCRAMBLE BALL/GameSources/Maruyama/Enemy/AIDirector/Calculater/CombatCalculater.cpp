
/*!
@file CombatCalculater.cpp
@brief CombatCalculaterのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "CombatCalculater.h"

#include "Watanabe/Component/PlayerStatus.h"
#include "Maruyama/Utility/Component/Targeted.h"

namespace basecross {

	namespace Enemy {

		namespace Calculater {


			float Combat::CalculateEvalutionValue(
				const std::shared_ptr<GameObject>& selfObject,
				const std::shared_ptr<GameObject>& target
			) {
				return CalculateEvalutionValue(selfObject->GetComponent<Transform>(), target);
			}

			float Combat::CalculateEvalutionValue(
				const std::shared_ptr<Transform>& selfTransform,
				const std::shared_ptr<GameObject>& target
			) {
				float result = 0.0f;

				auto targetTrans = target->GetComponent<Transform>();

				float toTargetRange = (targetTrans->GetPosition() - selfTransform->GetPosition()).length();
				result += toTargetRange;	//距離を評価値とする。

				//ターゲット本体の優先度を追加
				if (auto targeted = target->GetComponent<Targeted>(false)) {
					result *= targeted->GetPriority();
				}

				//playerステータスがある場合
				if (auto status = target->GetComponent<PlayerStatus>(false)) {
					float hpRate = (float)status->GetStatus().hp / (float)status->GetStatus().maxHp;
					result *= hpRate;	//hpが低いほど、距離を減衰(ターゲットにしやすくする。)
				}

				return result;
			}

		}
	}
}