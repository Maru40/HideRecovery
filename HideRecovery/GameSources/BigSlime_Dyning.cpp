
/*!
@file BigSlime_Dyning.cpp
@brief BigSlime_Dyningのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "PressData.h"
#include "TaskList.h"
#include "GameTimer.h"
#include "Task_Preliminary_NormalSlime.h"
#include "Task_BodyBlow.h"
#include "Task_Wait.h"

#include "EnemyStateNodeBase.h"

#include "BigSlime_Dyning.h"

#include "TargetManager.h"
#include "EnemyBase.h"
#include "I_Damaged.h"
#include "EnemyMainStateMachine.h"
#include "StatorBase.h"
#include "SlimeStatorBase.h"
#include "Stator_NormalSlime.h"
#include "Stator_ShootSlime.h"

#include "VelocityManager.h"
#include "CollisionAction.h"

#include "AbsorbedSlimeManager.h"

#include "EnemyObjectBase.h"
#include "SlimeObjectBase.h"

#include "TimeHelper.h"
#include "SeekTarget.h"
#include "MetaballRenderBase.h"
#include "MetaballChildrenRender.h"
#include "UtilityVelocity.h"
#include "VelocityManager.h"

#include "SingletonComponent.h"
#include "EffectManager.h"

#include "Targeted.h"

#include "Pusher.h"
#include "InvolvedManager.h"
#include "InvolvedObjects.h"

#include "PressData.h"
#include "EnemyBase.h"
#include "TargetManager.h"
#include "Slime_Dyning.h"

#include "DebugObject.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// パラメータ
			//--------------------------------------------------------------------------------------

			BigSlime_Dyning_Parametor::BigSlime_Dyning_Parametor()
				:BigSlime_Dyning_Parametor(0.5f, 2.0f, PressData())
			{}

			BigSlime_Dyning_Parametor::BigSlime_Dyning_Parametor(const float& time, const float& pressPower, const PressData& pressData)
				: time(time), pressPower(pressPower), pressData(pressData)
			{}

			//--------------------------------------------------------------------------------------
			/// 巨大スライムの死亡ステート本体
			//--------------------------------------------------------------------------------------

			BigSlime_Dyning::BigSlime_Dyning(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr)
				:EnemyStateNodeBase(owner), m_paramPtr(paramPtr), m_timer(new GameTimer(0.0f))
			{
				AddChangeComponent(owner->GetGameObject()->GetComponent<Targeted>(false), false, false);
			}

			void BigSlime_Dyning::OnStart() {
				StartChangeComponents();

				//タイマーのセット
				constexpr float plussDestroyTime = 1.0f; //将来的に変数化
				m_timer->ResetTimer(m_paramPtr->time + plussDestroyTime);
			}

			bool BigSlime_Dyning::OnUpdate() {
				m_timer->UpdateTimer();

				return m_timer->IsTimeUp();
			}

			void BigSlime_Dyning::OnExit() {
				ExitChangeComponents();

				GetOwner()->GetStage()->RemoveGameObject<GameObject>(GetOwner()->GetGameObject());
			}
		}
	}
}