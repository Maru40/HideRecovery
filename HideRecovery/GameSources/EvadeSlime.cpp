/*!
@file EvateSlime.cpp
@brief EvateSlimeのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"


#include "EnemyBase.h"
#include "I_KnockBack.h"
#include "I_PushedWater.h"
#include "SlimeBase.h"
#include "EvadeSlime.h"

#include "EnemyMainStateMachine.h"
#include "StatorBase.h"
#include "SlimeStatorBase.h"
#include "Stator_EvadeSlime.h"

#include "TaskList.h"
#include "SeekTarget.h"
#include "MaruAnimationHelper.h"
#include "MetaballChildrenObject.h"
#include "MetaballChildrenRender.h"
#include "MetaballMotionManager.h"


#include "AnimationMainStateMachine.h"
#include "AnimatorBase.h"
#include "Animator_EvadeSlime.h"

#include "I_KnockBack.h"

#include "Motion_KnockBack.h"

#include "EnemyStatusBase.h"

#include "PressData.h"
#include "VelocityManager.h"
#include "Motion_KnockBack.h"
#include "Slime_Dyning.h"
#include "MetaballChildrenPressedManager.h"

namespace basecross {
	namespace Enemy {

		namespace EvadeSlime {

			//--------------------------------------------------------------------------------------
			/// 逃げるスライムの本体
			//--------------------------------------------------------------------------------------

			EvadeSlime::EvadeSlime(const std::shared_ptr<GameObject>& objPtr)
				:SlimeBase(objPtr)
			{}

			//--------------------------------------------------------------------------------------
			/// インターフェースの実装
			//--------------------------------------------------------------------------------------

			void EvadeSlime::KnockBack(const KnockBackData& data) {
				auto statusManager = GetGameObject()->GetComponent<EnemyStatusManagerBase>(false);
				if (statusManager && !statusManager->IsToughnessBreak(data.power)) {
					return;
				}

				auto animator = GetGameObject()->GetComponent<Animator>(false);
				if (animator) {
					auto& blackBoard = animator->GetRefBlackBoard();
					blackBoard.knockBackParamPtr->data = data;
				}

				auto stator = GetGameObject()->GetComponent<Stator_EvadeSlime>(false);
				if (stator) {
					stator->GetTransitionMember().knockBackTrigger.Fire();
				}
			}

			PressedEnergy EvadeSlime::Pressed(const PressData& pressData) {
				auto pressManager = GetGameObject()->GetComponent<Metaball::ChildrenPressedManager>(false);
				auto stator = GetGameObject()->GetComponent<Stator_EvadeSlime>(false);
				if (stator && pressManager) {
					stator->ChangeState(Stator_EvadeSlime::State::Pressed, int(Stator_EvadeSlime::State::Pressed));

					//Slime_DeathのPressDataを設定
					auto blackBoard = stator->GetRefBlackBoardParam();
					const auto& deathParamPtr = blackBoard.deathParamPtr;
					deathParamPtr->pressData = pressData;
					deathParamPtr->pressPower = pressManager->GetPressedPower();
				}

				auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);
				auto animator = GetGameObject()->GetComponent<Animator>(false);
				if (animator && velocityManager) {
					auto& blackBoard = animator->GetRefBlackBoard();
					KnockBackData data;
					data.hitPoint = pressData.collisionPair.m_CalcHitPoint;
					data.direct = velocityManager->GetVelocity().GetNormalized();
					data.power = 100.0f;
					blackBoard.pressedParamPtr->range = 1.25f;
					blackBoard.pressedParamPtr->transitionTime = 0.075f;
					blackBoard.pressedParamPtr->data = data;
				}

				return SlimeBase::Pressed(pressData);
			}

		}

	}
}