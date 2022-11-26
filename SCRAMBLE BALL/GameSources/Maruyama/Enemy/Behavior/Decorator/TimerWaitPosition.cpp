/*!
@file TimerWaitPosition.cpp
@brief TimerWaitPositionなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "TimerWaitPosition.h"

#include "Maruyama/Utility/Timer/GameTimer.h"
#include "Maruyama/Interface/I_FactionMember.h"
#include "Maruyama/Enemy/AIDirector/CoordinatorBase.h"
#include "Maruyama/Enemy/AIDirector/FactionCoordinator.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// 一定時間同じ場所にとどまっていたら一度タスクをリセットするために、戻るデコレータのパラメータ
				//--------------------------------------------------------------------------------------

				TimerWaitPosition_Parametor::TimerWaitPosition_Parametor():
					time(3.0f),
					updateRange(2.0f)
				{}

				//--------------------------------------------------------------------------------------
				/// 一定時間同じ場所にとどまっていたら一度タスクをリセットするために、戻るデコレータ
				//--------------------------------------------------------------------------------------

				TimerWaitPosition::TimerWaitPosition(const std::shared_ptr<GameObject>& owner) :
					DecoratorBase(owner),
					m_param(Parametor()),
					m_timer(new GameTimer(m_param.time))
				{
					m_transform = owner->GetComponent<Transform>();
				}

				void TimerWaitPosition::OnStart() {
					UpdatePosition();
					m_timer->ResetTimer(m_param.time);
				}

				bool TimerWaitPosition::CanUpdate()  {
					m_timer->UpdateTimer();

					//登録されたポジションより一定距離離れたら、ポジションを更新、時間も更新
					if (IsUpdatePosition()) {
						UpdatePosition();
						m_timer->ResetTimer(m_param.time);
					}

					bool isTimeUp = m_timer->IsTimeUp();
					if (isTimeUp) {
						//ここにターゲット依頼行動を入れるといいかも？
						//auto factionMember = GetOwner()->GetComponent<Enemy::I_FactionMember>(false);
						//if (factionMember) {
						//	auto faction = factionMember->GetFactionCoordinator();
						//	if (faction) {
						//		//ターゲットを別にしてもらうようにお願いする。
						//	}
						//}

						Debug::GetInstance()->Log(L"TimeUp: 一定時間同じ場所にいました。");
					}

					return !isTimeUp;	//時間が終了したら、更新処理を終える。
				}

				void TimerWaitPosition::UpdatePosition() {
					m_observePosition = m_transform.lock()->GetPosition();
				}

				bool TimerWaitPosition::IsUpdatePosition() {
					auto toObservePosition = m_observePosition - m_transform.lock()->GetPosition();
					auto toRange = toObservePosition.length();	//距離を取得

					return m_param.updateRange < toRange;	//設定距離より離れたら、true
				}
			}
		}
	}
}