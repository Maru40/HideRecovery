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

#include "Maruyama/Utility/Timer/GameTimer.h"
#include "Maruyama/Utility/ObserveIsInEyeTarget.h"

#include "Maruyama/Utility/Utility.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// 監視対象が視界範囲にいるかどうかを判断するデコレータのパラメータ
				//--------------------------------------------------------------------------------------

				IsInEyeTarget_Parametor::IsInEyeTarget_Parametor():
					IsInEyeTarget_Parametor(10.0f, 20.0f)
				{}

				IsInEyeTarget_Parametor::IsInEyeTarget_Parametor(const float lostIntervalTime, const float farRange):
					lostIntervalTime(lostIntervalTime),
					farRange(farRange)
				{}

				//--------------------------------------------------------------------------------------
				/// 監視対象が視界範囲にいるかどうかを判断するデコレータ本体
				//--------------------------------------------------------------------------------------

				IsInEyeTarget::IsInEyeTarget(const std::shared_ptr<Enemy::EnemyBase>& owner) :
					IsInEyeTarget(owner, std::vector<std::weak_ptr<GameObject>>())
				{}

				IsInEyeTarget::IsInEyeTarget(
					const std::shared_ptr<Enemy::EnemyBase>& owner,
					const ObserveSharedTargets& observeTargets,
					const Parametor& parametor
				):
					IsInEyeTarget(
						owner, 
						maru::Utility::ConvertArraySharedToWeak(observeTargets),
						parametor
					)
				{}

				IsInEyeTarget::IsInEyeTarget(
					const std::shared_ptr<Enemy::EnemyBase>& owner,
					const ObserveTargets& observeTargets,
					const Parametor& parametor
				):
					DecoratorBase(owner),
					m_param(parametor),
					m_timer(new GameTimer(0.0f)),
					m_observeIsInTarget(new ObserveIsInEyeTarget(owner->GetGameObject()->GetComponent<EyeSearchRange>(), observeTargets))
				{
					m_eyeRange = owner->GetGameObject()->GetComponent<EyeSearchRange>(false);
					m_targetManager = owner->GetGameObject()->GetComponent<TargetManager>(false);
				}

				bool IsInEyeTarget::CanTransition() const {
					auto target = m_observeIsInTarget->SearchIsInEyeTarget();
					
					return target ? true : false;	//ターゲットが存在するならtrue
				}

				bool IsInEyeTarget::CanUpdate() {
					//ターゲットが存在しないなら、更新できない。
					auto targetManager = m_targetManager.lock();
					if (!targetManager || !targetManager->HasTarget()) {
						return false;
					}

					//ターゲットがかなり遠くにいるなら、追従をあきらめる。
					if (IsFarRange()) {
						return false;
					}

					//ターゲットを見失って、一定時間たったら、Updateを終了
					if (IsLost()) {
						//見失った後に一定時間たったら
						m_timer->UpdateTimer();
						if (m_timer->IsTimeUp()) {
							return false;
						}
					}
					else {
						m_timer->ResetTimer(m_param.lostIntervalTime);
					}

					return true;
				}

				bool IsInEyeTarget::IsLost() const {
					auto targetManager = m_targetManager.lock();
					if (!targetManager || !targetManager->HasTarget()) {
						return true;	//ターゲットが存在しないならtrue
					}

					auto targetPosition = targetManager->GetTargetPosition();
					if (m_eyeRange.lock()->IsInEyeRange(targetPosition)) {
						return false;	//ターゲットが視界内にいるならfalse(Lostしてない)
					}

					return true;		//視界内にいないため、Lost
				}

				bool IsInEyeTarget::IsFarRange() const {
					auto targetManager = m_targetManager.lock();
					if (!targetManager || !targetManager->HasTarget()) {
						return true;	//ターゲットが存在しないならtrue
					}

					auto toTargetRange = targetManager->CalcuToTargetVec().length();

					return m_param.farRange < toTargetRange;	//farRangeよりターゲットが遠くにいるならtrue
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