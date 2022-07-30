/*!
@file NormalSlime.cpp
@brief NormalSlimeのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyBase.h"
#include "I_KnockBack.h"
#include "I_PushedWater.h"
#include "SlimeBase.h"
#include "NormalSlime.h"

#include "I_KnockBack.h"

#include "EnemyMainStateMachine.h"
#include "StatorBase.h"
#include "SlimeStatorBase.h"
#include "Stator_NormalSlime.h"

#include "AnimationMainStateMachine.h"
#include "AnimatorBase.h"
#include "Animator_NormalSlime.h"

#include "Motion_KnockBack.h"

#include "PressCenterManager.h"

#include "EnemyStatusBase.h"
#include "VelocityManager.h"

#include "Slime_Dyning.h"

#include "MetaballChildrenPressedManager.h"
#include "TaskList.h"
#include "SeekTarget.h"
#include "MetaballChildrenRender.h"
#include "MetaballMotionManager.h"

namespace basecross {
	namespace Enemy {

		namespace NormalSlime {

			//--------------------------------------------------------------------------------------
			/// 通常スライムの本体
			//--------------------------------------------------------------------------------------

			NormalSlime::NormalSlime(const std::shared_ptr<GameObject>& objPtr)
				:SlimeBase(objPtr)
			{}

			//--------------------------------------------------------------------------------------
			/// インターフェースの実装
			//--------------------------------------------------------------------------------------

			void NormalSlime::KnockBack(const KnockBackData& data) {
				auto statusManager = GetGameObject()->GetComponent<EnemyStatusManagerBase>(false);
				if (statusManager && !statusManager->IsToughnessBreak(data.power)) {
					return;
				}

				if (auto animator = GetGameObject()->GetComponent<Animator>(false)) {
					auto& blackBoard = animator->GetRefBlackBoard();
					blackBoard.knockBackParamPtr->data = data;
				}

				if (auto stator = GetGameObject()->GetComponent<Stator_NormalSlime>(false)) {
					stator->GetTransitionMember().knockBackTrigger.Fire();
				}
			}

			PressedEnergy NormalSlime::Pressed(const PressData& pressData) {
				//ステートの制御
				auto pressManager = GetGameObject()->GetComponent<Metaball::ChildrenPressedManager>(false);
				auto stator = GetGameObject()->GetComponent<Stator_NormalSlime>(false);
				if (stator && pressManager) {
					//ステートの変更
					stator->ChangeState(Stator_NormalSlime::State::Pressed, int(Stator_NormalSlime::State::Pressed));

					//Slime_DeathのPressDataを設定
					auto blackBoard = stator->GetRefBlackBoardParam();
					const auto& deathParamPtr = blackBoard.deathParamPtr;
					deathParamPtr->pressData = pressData;
					deathParamPtr->pressPower = pressManager->GetPressedPower();
				}

				//アニメータの制御
				auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);
				auto animator = GetGameObject()->GetComponent<Animator>(false);
				if (animator && velocityManager) {
					//アニメータのノックバックデータを変更
					auto& blackBoard = animator->GetRefBlackBoard();
					auto pressMotionData = GetPressedMotionData(pressData);
					*blackBoard.pressedParamPtr = pressMotionData;
				}

				return SlimeBase::Pressed(pressData);
			}

		}

	}
}