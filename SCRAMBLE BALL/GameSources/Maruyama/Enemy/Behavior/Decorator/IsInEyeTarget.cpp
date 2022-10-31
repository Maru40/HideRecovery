/*!
@file IsInEyeTarget.cpp
@brief IsInEyeTargetなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "IsInEyeTarget.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Enemy/Component/EyeSearchRange.h"
#include "Maruyama/Utility/Component/TargetManager.h"
#include "Maruyama/Utility/ObserveIsInEyeTarget.h"

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
					m_observeIsInTarget(new ObserveIsInEyeTarget(owner->GetGameObject()->GetComponent<EyeSearchRange>(), observeTargets))
				{}

				bool IsInEyeTarget::CanTransition() const {
					auto target = m_observeIsInTarget->SearchIsInEyeTarget();
					
					return target ? true : false;	//ターゲットが存在するならtrue
				}

				bool IsInEyeTarget::CanUpdate() {
					//ターゲットを見失って、一定時間たったら、Updateを終了

					return true;
				}

				//--------------------------------------------------------------------------------------
				/// アクセッサ
				//--------------------------------------------------------------------------------------

				void IsInEyeTarget::AddObserveTarget(const std::shared_ptr<GameObject>& target) {
					m_observeIsInTarget->AddObserveTarget(target);
				}

				void IsInEyeTarget::SetObserveTargets(const ObserveTargets& targets) {
					m_observeIsInTarget->SetObserveTargets(targets); 
				}

				void IsInEyeTarget::SetObserveTargets(const ObserveSharedTargets& targets) {
					SetObserveTargets(maru::Utility::ConvertArraySharedToWeak(targets));
				}

				_NODISCARD IsInEyeTarget::ObserveTargets IsInEyeTarget::GetObserveTargets() const noexcept {
					return m_observeIsInTarget->GetObserveTargets();
				}

			}
		}
	}
}