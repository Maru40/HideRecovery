
/*!
@file Slime_Pressed.cpp
@brief Slime_Pressedのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyBase.h"
#include "TargetManager.h"
#include "Slime_Pressed.h"

#include "SlowTimerManager.h"
#include "HitStopManager.h"
#include "MetaballChildrenPressedManager.h"

#include "TimeHelper.h"
#include "GameTimer.h"

#include "MetaballRenderBase.h"

#include "MetaballChildrenObject.h"
#include "MetaballChildrenRender.h"

#include "SlimeCore.h"
#include "Targeted.h"

#include "AutoReflection.h"

#include "AbsorbedSlimeManager.h"

#include "TargetManager.h"
#include "EnemyBase.h"
#include "I_Damaged.h"
#include "EnemyMainStateMachine.h"
#include "StatorBase.h"
#include "SlimeStatorBase.h"
#include "Stator_NormalSlime.h"
#include "Stator_ShootSlime.h"

#include "Pusher.h"
#include "InvolvedManager.h"

#include "PressData.h"
#include "EnemyBase.h"
#include "TargetManager.h"
#include "Slime_Dyning.h"

#include "HitStopManager.h"
#include "PressData.h"
#include "PressCenterManager.h"

#include "EnemyObjectBase.h"
#include "SlimeObjectBase.h"

#include "Stator_BigSlime.h"
#include "Stator_BigShootSlime.h"

#include "BigSlime_Dyning.h"

#include "SeekTarget.h"
#include "VelocityManager.h"

#include "TaskList.h"
#include "SeekTarget.h"
#include "MetaballChildrenRender.h"
#include "MetaballMotionManager.h"
#include "AnimationMainStateMachine.h"
#include "AnimatorBase.h"
#include "Animator_NormalSlime.h"
#include "Animator_ShootSlime.h"

#include "I_KnockBack.h"
#include "MaruAnimationHelper.h"
#include "Motion_KnockBack.h"

#include "SingletonComponent.h"
#include "EffectManager.h"

#include "I_Pressed.h"

#include "UtilityVelocity.h"

#include "ShootManager.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			Slime_Pressed::Slime_Pressed(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr)
				:EnemyStateNodeBase<EnemyBase>(owner), m_paramPtr(paramPtr), m_timer(new GameTimer(0.0f))
			{
				auto ownerObject = owner->GetGameObject();
				AddChangeComponent(ownerObject->GetComponent<AutoReflection>(false), false, false);
				AddChangeComponent(ownerObject->GetComponent<Targeted>(false), false, false);
				AddChangeComponent(ownerObject->GetComponent<CollisionObb>(false), false, false);

				//追加
				AddChangeComponent(ownerObject->GetComponent<Gravity>(false), false, false);
				AddChangeComponent(ownerObject->GetComponent<PressCenterManager>(false), false, false);
				AddChangeComponent(ownerObject->GetComponent<ShootSlime::ShootManager>(false), false, false);
				AddChangeComponent(ownerObject->GetComponent<VelocityManager>(false), false, false);
			}

			void Slime_Pressed::OnStart() {
				StartChangeComponents();

				if (m_paramPtr->m_startAnimation) {
					m_paramPtr->m_startAnimation();
				}

				auto pressManager = GetOwner()->GetGameObject()->GetComponent<Metaball::ChildrenPressedManager>(false);
				if (pressManager) {
					m_timer->ResetTimer(pressManager->GetPressedTime());
				}

				//Coreを消す
				auto core = GetOwner()->GetGameObject()->GetComponent<SlimeCore>(false);
				if (core) {
					core->SetDrawActive(false);
				}
			}

			bool Slime_Pressed::OnUpdate() {
				m_timer->UpdateTimer(1.0f, maru::DeltaType::Real);

				return m_timer->UpdateTimer();
			}

			void Slime_Pressed::OnExit() {
				ExitChangeComponents();

				if (m_paramPtr->m_endAnimation) {
					m_paramPtr->m_endAnimation();
				}
			}
		}
	}
}