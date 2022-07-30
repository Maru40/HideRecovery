/*!
@file ShootSlime.cpp
@brief ShootSlimeのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyBase.h"
#include "I_KnockBack.h"
#include "I_PushedWater.h"
#include "SlimeBase.h"
#include "ShootSlime.h"

#include "EnemyMainStateMachine.h"
#include "StatorBase.h"
#include "SlimeStatorBase.h"
#include "Stator_ShootSlime.h"

#include "EnemyStatusBase.h"

#include "AnimationMainStateMachine.h"
#include "AnimatorBase.h"
#include "Animator_ShootSlime.h"

#include "PressData.h"
#include "VelocityManager.h"
#include "Motion_KnockBack.h"
#include "Slime_Dyning.h"
#include "MetaballChildrenPressedManager.h"

#include "TaskList.h"
#include "SeekTarget.h"
#include "MetaballChildrenRender.h"
#include "MetaballMotionManager.h"

namespace basecross {
	namespace Enemy {

		namespace ShootSlime {

			//--------------------------------------------------------------------------------------
			/// ショットスライム本体
			//--------------------------------------------------------------------------------------

			ShootSlime::ShootSlime(const std::shared_ptr<GameObject>& objPtr)
				:SlimeBase(objPtr)
			{}

			//--------------------------------------------------------------------------------------
			/// インターフェース
			//--------------------------------------------------------------------------------------

			void ShootSlime::KnockBack(const KnockBackData& data) {
				auto statusManager = GetGameObject()->GetComponent<EnemyStatusManagerBase>(false);
				if (statusManager && !statusManager->IsToughnessBreak(data.power)) {
					return;
				}

				auto animator = GetGameObject()->GetComponent<Animator>(false);
				if (animator) {
					animator->ChangeState(Animator::StateType::KnockBack);
				}

				auto stator = GetGameObject()->GetComponent<Stator_ShootSlime>(false);
				if (stator) {
					stator->GetTransitionMember().knockBackTrigger.Fire();
				}
			}

			PressedEnergy ShootSlime::Pressed(const PressData& pressData) {
				auto pressManager = GetGameObject()->GetComponent<Metaball::ChildrenPressedManager>(false);
				auto stator = GetGameObject()->GetComponent<Stator_ShootSlime>(false);
				if (stator && pressManager) {
					stator->ChangeState(Stator_ShootSlime::State::Pressed, int(Stator_ShootSlime::State::Pressed));

					//Slime_DeathのPressDataを設定
					auto blackBoard = stator->GetRefBlackBoardParam();
					const auto& deathParamPtr = blackBoard.deathParamPtr;
					deathParamPtr->pressData = pressData;
					deathParamPtr->pressPower = pressManager->GetPressedPower();
				}

				auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);
				auto animator = GetGameObject()->GetComponent<Animator>(false);
				if (animator && velocityManager) {
					//アニメータのノックバックモーションのデータを変更
					auto& blackBoard = animator->GetRefBlackBoard();
					*blackBoard.pressedParamPtr = GetPressedMotionData(pressData);
				}

				return SlimeBase::Pressed(pressData);
			}

		}
	}
}