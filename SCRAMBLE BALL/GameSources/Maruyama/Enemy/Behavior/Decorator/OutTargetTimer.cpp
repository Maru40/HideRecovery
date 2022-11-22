/*!
@file SettingStartTarget.cpp
@brief SettingStartTargetなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Utility/Component/Targeted.h"
#include "Maruyama/Utility/Timer/GameTimer.h"
#include "Maruyama/Utility/Component/TargetManager.h"
#include "Maruyama/Enemy/Component/EyeSearchRange.h"

#include "Maruyama/Utility/Random.h"

#include "OutTargetTimer.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// ターゲットが視界外に一定時間いると、強制終了させるデコレータのパラメータ
				//--------------------------------------------------------------------------------------

				OutTargetTimer_Parametor::OutTargetTimer_Parametor() :
					OutTargetTimer_Parametor(EyeSearchRangeParametor())
				{}

				OutTargetTimer_Parametor::OutTargetTimer_Parametor(const EyeSearchRangeParametor& eyeParametor) :
					OutTargetTimer_Parametor(eyeParametor, 10.0f, 15.0f)
				{}

				OutTargetTimer_Parametor::OutTargetTimer_Parametor(
					const EyeSearchRangeParametor& eyeParametor,
					const float minLostIntervalTime,
					const float maxLostIntervalTime
				) :
					OutTargetTimer_Parametor(eyeParametor, minLostIntervalTime, maxLostIntervalTime, 30.0f)
				{}

				OutTargetTimer_Parametor::OutTargetTimer_Parametor(
					const EyeSearchRangeParametor& eyeParametor,
					const float minLostIntervalTime,
					const float maxLostIntervalTime,
					const float farRange
				) :
					eyeParametor(eyeParametor),
					minLostIntervalTime(minLostIntervalTime),
					maxLostIntervalTime(maxLostIntervalTime),
					farRange(farRange)
				{}

				//--------------------------------------------------------------------------------------
				/// ターゲットが視界外に一定時間いると、強制終了させるデコレータ
				//--------------------------------------------------------------------------------------

				OutTargetTimer::OutTargetTimer(const std::shared_ptr<Enemy::EnemyBase>& owner, const Parametor* paramPtr):
					DecoratorBase(owner),
					m_timer(new GameTimer(0.0f)),
					m_paramPtr(paramPtr)
				{
					auto object = owner->GetGameObject();

					m_eyeRange = object->GetComponent<EyeSearchRange>(false);
					m_targetManager = object->GetComponent<TargetManager>(false);
				}

				void OutTargetTimer::OnStart() {
					m_timer->ResetTimer(GetRandomIntervalTime());
				}

				bool OutTargetTimer::CanUpdate() {
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
						m_timer->ResetTimer(GetRandomIntervalTime());
					}

					return true;
				}

				float OutTargetTimer::GetRandomIntervalTime() const {
					return maru::MyRandom::Random(m_paramPtr->minLostIntervalTime, m_paramPtr->maxLostIntervalTime);
				}

				bool OutTargetTimer::IsFarRange() const {
					auto targetManager = m_targetManager.lock();
					if (!targetManager || !targetManager->HasTarget()) {
						return true;	//ターゲットが存在しないならtrue
					}

					auto toTargetRange = targetManager->CalcuToTargetVec().length();

					return m_paramPtr->farRange < toTargetRange;	//farRangeよりターゲットが遠くにいるならtrue
				}

				bool OutTargetTimer::IsLost() const {
					auto targetManager = m_targetManager.lock();
					if (!targetManager || !targetManager->HasTarget()) {
						return true;	//ターゲットが存在しないならtrue
					}

					if (m_eyeRange.lock()->IsInEyeRange(targetManager->GetTarget(), m_paramPtr->eyeParametor)) {
						return false;	//ターゲットが視界内にいるならfalse(Lostしてない)
					}

					return true;		//視界内にいないため、Lost
				}

			}
		}
	}
}