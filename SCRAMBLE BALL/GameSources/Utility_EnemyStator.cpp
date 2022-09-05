
/*!
@file Utility_EnemyStator.cpp
@brief Utility_EnemyStatorのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Utility_EnemyStator.h"

#include "Maruyama/Enemy/Component/EyeSearchRange.h"
#include "TargetManager.h"
#include "VelocityManager.h"

#include "GraphBase.h"
#include "Maruyama/Enemy/StateMachine/StateNodeBase.h"
#include "Maruyama/Enemy/StateMachine/StateEdgeBase.h"
#include "MaruAnimationHelper.h"
#include "Maruyama/Enemy/Component/Animation/AnimationMainStateMachine.h"

#include "GameStageBase.h"

#include "Maruyama/Debug/Object/DebugObject.h"

namespace basecross {
	namespace Enemy {
		namespace UtilityStator {

			//プレイヤーが視界に入った時の遷移判定
			bool Transition::InEyeRangePlayer(const std::shared_ptr<GameObject>& selfObject, const float& eyeRange) {
				auto eye = selfObject->GetComponent<EyeSearchRange>(false);
				auto targetManager = selfObject->GetComponent<TargetManager>(false);
				if (!eye || !targetManager) {
					return false;
				}

				std::shared_ptr<GameObject> target = selfObject->GetGameStage()->GetPlayer();

				if (eye->IsInEyeRange(target, eyeRange)) {
					targetManager->SetTarget(target);
					return true;
				}

				return false;
			}

			//ターゲットが視界に入った時の遷移判定
			bool Transition::InEyeRangeTarget(const std::shared_ptr<GameObject>& selfObject, const float& eyeRange) {
				auto eye = selfObject->GetComponent<EyeSearchRange>(false);
				auto targetManager = selfObject->GetComponent<TargetManager>(false);
				if (!eye || !targetManager) {
					return false;
				}

				auto target = targetManager->GetTarget();
				return eye->IsInEyeRange(target, eyeRange);
			}

			//ターゲットをロストした遷移判定
			bool Transition::LostTarget(const std::shared_ptr<GameObject>& selfObject, const float& maxChaseRange) {
				auto eye = selfObject->GetComponent<EyeSearchRange>(false);
				auto targetManager = selfObject->GetComponent<TargetManager>(false);
				//欲しいコンポーネントが無かったら
				if (!eye || !targetManager) {
					DebugObject::AddString(L"LostTargetで、遷移するためのコンポーネントが足りません。");
					return false;
				}

				//ターゲットがNullなら遷移する。
				if (!targetManager->HasTarget()) {
					return true;
				}

				//範囲外だったら追従をやめる
				auto toTargetVec = targetManager->CalcuToTargetVec();
				if (toTargetVec.length() > maxChaseRange) {
					targetManager->SetTarget(nullptr);
					return true;
				}

				return false;
			}

			bool Transition::ComparisonVelocity(const std::shared_ptr<GameObject>& selfObject,
				const float& transitionVelocity, 
				const ComparisonType& type)
			{
				auto velocityManager = selfObject->GetComponent<VelocityManager>(false);
				if (!velocityManager) {
					return true;
				}

				switch (type)
				{
				case ComparisonType::Less:
					return velocityManager->GetWorldVelocity().length() < transitionVelocity;
					break;

				case ComparisonType::OrLess:
					return velocityManager->GetWorldVelocity().length() <= transitionVelocity;
					break;

				case ComparisonType::Greater:
					return velocityManager->GetWorldVelocity().length() > transitionVelocity;
					break;

				case ComparisonType::OrGreater:
					return velocityManager->GetWorldVelocity().length() >= transitionVelocity;
					break;
				}

				return true;
			}
		}
	}
}