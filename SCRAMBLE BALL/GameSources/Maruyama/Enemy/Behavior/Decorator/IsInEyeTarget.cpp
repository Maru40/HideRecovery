/*!
@file I_Decorator.cpp
@brief I_Decoratorなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "IsInEyeTarget.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Enemy/Component/EyeSearchRange.h"
#include "Maruyama/Utility/Component/TargetManager.h"

#include "Maruyama/Utility/Utility.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				IsInEyeTarget::IsInEyeTarget(const std::shared_ptr<Enemy::EnemyBase>& owner) :
					IsInEyeTarget(owner, std::vector<std::weak_ptr<GameObject>>())
				{}

				IsInEyeTarget::IsInEyeTarget(
					const std::shared_ptr<Enemy::EnemyBase>& owner,
					const ObserveSharedTargets& observeTargets
				):
					IsInEyeTarget(owner, maru::Utility::ConvertArraySharedToWeak(observeTargets))
				{}

				IsInEyeTarget::IsInEyeTarget(
					const std::shared_ptr<Enemy::EnemyBase>& owner,
					const ObserveTargets& observeTargets
				):
					DecoratorBase(owner),
					m_observeTargets(observeTargets)
				{
					m_eyeRange = owner->GetGameObject()->GetComponent<EyeSearchRange>(false);
				}

				bool IsInEyeTarget::CanTransition() const {
					auto eyeRange = m_eyeRange.lock();
					if (!eyeRange) {
						Debug::GetInstance()->Log(L"IsInEyeTarget::CanTransition() : 必要コンポーネントがありません。");
						return false;	//視界がないから判断できないため、遷移できない。
					}

					for (auto& weakTarget : m_observeTargets) {
						if (weakTarget.expired()) {
							continue;	//対象がないなら対象外
						}
						
						//視界範囲内なら、遷移できる。
						auto target = weakTarget.lock();
						auto targetPosition = target->GetComponent<Transform>()->GetPosition();
						if (eyeRange->IsInEyeRange(targetPosition)) {
							//本来は評価値をノード本体に渡す。
							return true;
						}
					}

					return false;
				}

				bool IsInEyeTarget::CanUpdate() {
					//ターゲットを見失って、一定時間たったら、Updateを終了

					return true;
				}

				//--------------------------------------------------------------------------------------
				/// アクセッサ
				//--------------------------------------------------------------------------------------

				void IsInEyeTarget::SetObserveTargets(const ObserveSharedTargets& targets) {
					SetObserveTargets(maru::Utility::ConvertArraySharedToWeak(targets));
				}

			}
		}
	}
}