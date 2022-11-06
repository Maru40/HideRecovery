/*!
@file IsInEyeTargets.cpp
@brief IsInEyeTargetsなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "IsInEyeTarget.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Enemy/Component/EyeSearchRange.h"
#include "Maruyama/Utility/Component/TargetManager.h"

#include "Maruyama/Utility/Timer/GameTimer.h"

#include "Maruyama/Utility/Utility.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// 監視対象が視界範囲にいるかどうかを判断するデコレータのパラメータ
				//--------------------------------------------------------------------------------------

				IsInEyeTarget_Parametor::IsInEyeTarget_Parametor() :
					IsInEyeTarget_Parametor(EyeSearchRangeParametor())
				{}

				IsInEyeTarget_Parametor::IsInEyeTarget_Parametor(const EyeSearchRangeParametor& eyeParametor) :
					IsInEyeTarget_Parametor(eyeParametor, 10.0f)
				{}

				IsInEyeTarget_Parametor::IsInEyeTarget_Parametor(
					const EyeSearchRangeParametor& eyeParametor,
					const float lostIntervalTime
				):
					IsInEyeTarget_Parametor(eyeParametor, lostIntervalTime, 30.0f)
				{}

				IsInEyeTarget_Parametor::IsInEyeTarget_Parametor(
					const EyeSearchRangeParametor& eyeParametor,
					const float lostIntervalTime, 
					const float farRange
				) :
					eyeParametor(eyeParametor),
					lostIntervalTime(lostIntervalTime),
					farRange(farRange)
				{}

				//--------------------------------------------------------------------------------------
				/// 監視対象が視界範囲にいるかどうかを判断するデコレータ
				//--------------------------------------------------------------------------------------

				IsInEyeTarget::IsInEyeTarget(const std::shared_ptr<Enemy::EnemyBase>& owner, const Parametor* paramPtr):
					DecoratorBase(owner),
					m_paramPtr(paramPtr),
					m_timer(new GameTimer(0.0f))
				{
					m_eyeRange = owner->GetGameObject()->GetComponent<EyeSearchRange>(false);
					m_targetManager = owner->GetGameObject()->GetComponent<TargetManager>(false);
				}

				bool IsInEyeTarget::CanTransition() const {
					auto targetManager = m_targetManager.lock();
					if (!targetManager || !targetManager->HasTarget()) {
						return false;
					}

					return m_eyeRange.lock()->IsInEyeRange(targetManager->GetTarget(), m_paramPtr->eyeParametor.length);
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
						m_timer->ResetTimer(m_paramPtr->lostIntervalTime);
					}

					return true;
				}

				bool IsInEyeTarget::IsFarRange() const {
					auto targetManager = m_targetManager.lock();
					if (!targetManager || !targetManager->HasTarget()) {
						return true;	//ターゲットが存在しないならtrue
					}

					auto toTargetRange = targetManager->CalcuToTargetVec().length();

					return m_paramPtr->farRange < toTargetRange;	//farRangeよりターゲットが遠くにいるならtrue
				}

				bool IsInEyeTarget::IsLost() const {
					auto targetManager = m_targetManager.lock();
					if (!targetManager || !targetManager->HasTarget()) {
						return true;	//ターゲットが存在しないならtrue
					}

					auto targetPosition = targetManager->GetTargetPosition();
					if (m_eyeRange.lock()->IsInEyeRange(targetPosition), m_paramPtr->eyeParametor.length) {
						return false;	//ターゲットが視界内にいるならfalse(Lostしてない)
					}

					return true;		//視界内にいないため、Lost
				}

			}
		}
	}
}