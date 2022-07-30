/*!
@file Stator_BigSlime.cpp
@brief Stator_BigSlimeのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "StatorBase.h"
#include "SlimeStatorBase.h"
#include "Stator_BigSlime.h"

#include "EnemyBase.h"
#include "EnemyMainStateMachine.h"

#include "TimeHelper.h"
#include "GameTimer.h"
#include "TargetManager.h"
#include "TaskList.h"
#include "Task_Wait.h"
#include "Task_BodyBlow.h"
#include "EnemyStateNodeBase.h"
#include "Slime_Plowling.h"
#include "Slime_KnockBack.h"
#include "PressData.h"
#include "Slime_Pressed.h"
#include "Slime_Dyning.h"
#include "Slime_Absorbed.h"
#include "ChaseState_AstarSeek.h"
#include "ChaseState_TargetSeek.h"
#include "Task_MovePositions.h"
#include "Task_AroundEyeCheck.h"
#include "Slime_Chase.h"
#include "Slime_Attack.h"
#include "BigSlime_Attack.h"

#include "EvadeSlime_Evade.h"

#include "AnimationMainStateMachine.h"
#include "AnimatorBase.h"
#include "Animator_BigSlime.h"

#include "Utility_EnemyStator.h"

#include "BigSlime_Dyning.h"

#include "EyeSearchRange.h"
#include "GameManager.h"

namespace basecross {
	namespace Enemy {

		namespace BigSlime {

			using namespace StateNode;

			//--------------------------------------------------------------------------------------
			/// パラメータ
			//--------------------------------------------------------------------------------------
			struct Stator::Parametor
			{
				std::shared_ptr<Slime_Absorbed_Parametor> absorbedParamPtr;     //吸収されている状態
				std::shared_ptr<Slime_Plowling::Parametor> plowlingParamPtr;    //徘徊
				std::shared_ptr<Slime_Chase::Parametor> chaseParamPtr;          //追従
				std::shared_ptr<Slime_KnockBack::Parametor> knockBackParamPtr;  //ノックバック
				std::shared_ptr<BigSlime_Attack::Parametor> attackParamPtr;     //攻撃
				std::shared_ptr<Slime_Pressed::Parametor> pressedParamPtr;      //潰される
				std::shared_ptr<Slime_Dyning::Parametor> dyningParamPtr;        //死亡

				Parametor() :
					absorbedParamPtr(new Slime_Absorbed_Parametor()),
					plowlingParamPtr(new Slime_Plowling::Parametor()),
					chaseParamPtr(new Slime_Chase::Parametor()),
					knockBackParamPtr(new Slime_KnockBack::Parametor()),
					attackParamPtr(new BigSlime_Attack::Parametor()),
					pressedParamPtr(new Slime_Pressed::Parametor()),
					dyningParamPtr(new Slime_Dyning::Parametor())
				{}
			};

			//--------------------------------------------------------------------------------------
			/// 遷移条件メンバ
			//--------------------------------------------------------------------------------------

			Stator_TransitionMemeber::Stator_TransitionMemeber()
				:Stator_TransitionMemeber(10.0f, 15.0f, 6.0f)
			{}

			Stator_TransitionMemeber::Stator_TransitionMemeber(
				const float& startChaseRange, const float& maxChaseRange,
				const float& bodyBlowRange
			) :
				startChaseRange(startChaseRange), maxChaseRange(maxChaseRange),
				bodyBlowRange(bodyBlowRange)
			{}

			//--------------------------------------------------------------------------------------
			/// 遷移条件
			//--------------------------------------------------------------------------------------

			//タックル攻撃に遷移するかどうか
			bool Stator::IsBodyBlowRange(const Stator::TransitionMember& member) {
				auto eye = GetGameObject()->GetComponent<EyeSearchRange>(false);
				auto targetManager = GetGameObject()->GetComponent<TargetManager>(false);
				//欲しいコンポーネントが無かったら
				if (!eye || !targetManager) {
					DebugObject::AddString(L"Stator_Slimeで、BodyBlowに遷移するためのコンポーネントが足りません。");
					return false;
				}

				if (!targetManager->HasTarget()) {
					return false;
				}

				//ターゲットが視界内の範囲にいたら遷移する
				return eye->IsInEyeRange(targetManager->GetTarget(), member.bodyBlowRange);
			}

			//追従トリガー
			bool ToChaseTrigger(const Stator::TransitionMember& member) {
				return member.chaseTrigger.Get();
			}

			bool ToKnockBackTrigger(const Stator::TransitionMember& member) {
				return member.knockBackTrigger.Get();
			}

			//--------------------------------------------------------------------------------------
			/// 巨大スライムのステーター本体
			//--------------------------------------------------------------------------------------

			Stator::Stator(const std::shared_ptr<GameObject>& owner):
				Stator(owner, TransitionMember())
			{ }

			Stator::Stator(const std::shared_ptr<GameObject>& owner, const TransitionMember& member):
				SlimeStatorBase(owner, member), m_paramPtr(new Parametor())
			{
				GetRefBlackBoardParam().dyningParamPtr = m_paramPtr->dyningParamPtr;
			}

			void Stator::CreateNode() {
				auto enemy = GetGameObject()->GetComponent<EnemyBase>();
				auto animator = GetGameObject()->GetComponent<BigSlime::Animator>(false);
				SettingParametor();

				//Start(生成中)
				m_stateMachine->AddNode(State::Start, nullptr);

				//徘徊
				m_stateMachine->AddNode(State::Plowling, std::make_shared<StateNode::Slime_Plowling>(enemy, m_paramPtr->plowlingParamPtr));

				//追従
				m_stateMachine->AddNode(State::Chase, std::make_shared<StateNode::Slime_Chase>(enemy, m_paramPtr->chaseParamPtr));

				//ノックバック
				m_stateMachine->AddNode(State::KnockBack, std::make_shared<StateNode::Slime_KnockBack>(enemy, m_paramPtr->knockBackParamPtr));

				//攻撃
				m_stateMachine->AddNode(State::Attack, std::make_shared<StateNode::BigSlime_Attack>(enemy, m_paramPtr->attackParamPtr));  //新規作成

				//潰される
				m_stateMachine->AddNode(State::Pressed, std::make_shared<StateNode::Slime_Pressed>(enemy, m_paramPtr->pressedParamPtr));

				//死亡
				m_stateMachine->AddNode(State::Dyning, std::make_shared<StateNode::Slime_Dyning>(enemy, m_paramPtr->dyningParamPtr));  //新規生成

				//Death
				m_stateMachine->AddNode(State::Death, nullptr);

				//None
				m_stateMachine->AddNode(State::None, nullptr);
			}

			void Stator::CreateEdge() {
				using namespace UtilityStator;

				//Start
				m_stateMachine->AddEdge(State::Start, State::Plowling,
					[&](const TransitionMember& member) { return GameManager::GetInstance()->GetCurrentState() == GameManager::State::Game; });

				//徘徊
				m_stateMachine->AddEdge(State::Plowling, State::Chase, 
					[&, this](const TransitionMember& member) { return Transition::InEyeRangePlayer(GetGameObject(), member.startChaseRange); });
				m_stateMachine->AddEdge(State::Plowling, State::KnockBack, &ToKnockBackTrigger);

				//追従
				m_stateMachine->AddEdge(State::Chase, State::Plowling,
					[&, this](const TransitionMember& member) { return Transition::LostTarget(GetGameObject(), member.maxChaseRange); });
				m_stateMachine->AddEdge(State::Chase, State::Attack,
					[&, this](const TransitionMember& member) { return IsBodyBlowRange(member); });
				m_stateMachine->AddEdge(State::Chase, State::KnockBack, &ToKnockBackTrigger);

				//ノックバック
				m_stateMachine->AddEdge(State::KnockBack, State::KnockBack,
					[&, this](const TransitionMember& member) { return member.knockBackTrigger.Get(); });
				m_stateMachine->AddEdge(State::KnockBack, State::Plowling, &ToKnockBackTrigger);
				m_stateMachine->AddEdge(State::KnockBack, State::Plowling, [](const TransitionMember& member) { return true; }, true);

				//攻撃
				m_stateMachine->AddEdge(State::Attack, State::Chase, &ToChaseTrigger);
				m_stateMachine->AddEdge(State::Attack, State::KnockBack, &ToKnockBackTrigger);
				m_stateMachine->AddEdge(State::Attack, State::Chase, [](const TransitionMember& member) { return true; }, true);

				//潰される
				m_stateMachine->AddEdge(State::Pressed, State::Dyning,
					[](const TransitionMember& member) { return true; }, true);

				//死亡
				m_stateMachine->AddEdge(State::Dyning, State::Death,
					[](const TransitionMember& member) { return true; }, true);
			}

			//--------------------------------------------------------------------------------------
			/// パラメータセッティング
			//--------------------------------------------------------------------------------------

			void Stator::SettingParametor() {
				SettingPlowling();
				SettingChase();
			}

			void Stator::SettingPlowling() {
				auto& plowlingParamPtr = m_paramPtr->plowlingParamPtr;
				plowlingParamPtr->positions = {
					Vec3(+5.0f, +0.5f, +3.0f),
					Vec3(-5.0f, +0.5f, +3.0f),
					Vec3(+5.0f, +0.5f, -3.0f)
				};
				plowlingParamPtr->waitParamPtr->time = 1.0f;

				//移動パラメータ
				auto& moveParamPtr = plowlingParamPtr->moveParamPtr->moveParamPtr;
				moveParamPtr->speed = 5.0f;
				moveParamPtr->targetNearRange = 2.0f;
				moveParamPtr->moveType = Task_ToTargetMove::MoveType::ArriveVelocity;
			}

			void Stator::SettingChase() {
				auto& chaseParamPtr = m_paramPtr->chaseParamPtr;
				//探索時
				chaseParamPtr->astarSeekParamPtr->moveParamPtr->moveParamPtr->speed = 2.0f;
				//追従時
				chaseParamPtr->targetSeekParamPtr->seekParamPtr->speed = 5.0f;
			}

		}
	}
}