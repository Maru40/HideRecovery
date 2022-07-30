
/*!
@file Slime_Dyning.cpp
@brief Slime_Dyningのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "PressData.h"
#include "EnemyBase.h"
#include "TargetManager.h"
#include "Slime_Dyning.h"

#include "MetaballRenderBase.h"

#include "MetaballChildrenObject.h"
#include "MetaballChildrenRender.h"

#include "RayMarchingFilter.h"
#include "BcPNTSlimeDraw.h"

#include "TimeHelper.h"
#include "SeekTarget.h"

#include "VelocityManager.h"

#include "GameTimer.h"
#include "AutoReflection.h"

#include "UtilityVelocity.h"

#include "EnemyObjectBase.h"
#include "SlimeObjectBase.h"
#include "NormalSlimeObject.h"

#include "RandomPositionGenerator.h"
#include "Targeted.h"

#include "ScalePop.h"

#include "EffectManager.h"

#include "CrushScore.h"
#include "I_Pressed.h"
#include "PressCenterManager.h"

#include "MaruUtility.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// パラメータ
			//--------------------------------------------------------------------------------------

			Slime_Dyning_Parametor::Slime_Dyning_Parametor()
				:Slime_Dyning_Parametor(0.5f, 2.0f, PressData())
			{}

			Slime_Dyning_Parametor::Slime_Dyning_Parametor(const float& time, const float& pressPower, const PressData& pressData)
				:time(time), pressPower(pressPower), pressData(pressData)
			{}

			//--------------------------------------------------------------------------------------
			/// スライム死亡遷移ステート
			//--------------------------------------------------------------------------------------

			Slime_Dyning::Slime_Dyning(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr)
				:EnemyStateNodeBase<EnemyBase>(owner), m_paramPtr(paramPtr), m_timer(new GameTimer(0.0f))
			{
				AddChangeComponent(owner->GetGameObject()->GetComponent<Targeted>(false), false, false);
			}

			void Slime_Dyning::OnStart() {
				StartChangeComponents();

				//Successの表示
				CreateSuccessEffect();

				auto ownerObject = GetOwner()->GetGameObject();

				//Pressedで拡散処理を呼ぶ
				auto pressCenterManager = ownerObject->GetComponent<PressCenterManager>(false);
				auto pressed = ownerObject->GetComponent<I_Pressed>(false);
				if (pressed && pressCenterManager) { //二つのコンポ―ネントが存在するなら。
					pressed->Spread(pressCenterManager->GetSuccessPressData());
				}

				constexpr float plussDestroyTime = 1.0f; //将来的に変数化
				m_timer->ResetTimer(m_paramPtr->time + plussDestroyTime);
			}

			bool Slime_Dyning::OnUpdate() {
				m_timer->UpdateTimer();

				return m_timer->IsTimeUp();
			}

			void Slime_Dyning::OnExit() {
				ExitChangeComponents();

				GetOwner()->GetStage()->RemoveGameObject<GameObject>(GetOwner()->GetGameObject());
			}

			//エフェクトの生成
			void Slime_Dyning::CreateSuccessEffect() {
				auto pressCenterManager = GetOwner()->GetGameObject()->GetComponent<PressCenterManager>(false);
				if (!pressCenterManager) {
					return;
				}

				auto& pressData = pressCenterManager->GetSuccessPressData();
				auto crushScore = maru::Utility::FindGameObject<CrushScore>();
				if (crushScore)
				{
					switch (pressData.pressType)
					{
					case PressType::Perfect:
						crushScore->DeployUi(L"Great");
						break;

					case PressType::Good:
						crushScore->DeployUi(L"Good");
						break;
					}
				}
			}
		}
	}
}