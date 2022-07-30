
/*!
@file Stator_NormalSlime.cpp
@brief Stator_NormalSlimeのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Trigger.h"

#include "EnemyMainStateMachine.h"
#include "StatorBase.h"
#include "SlimeStatorBase.h"
#include "Stator_NormalSlime.h"

#include "EnemyBase.h"
#include "EyeSearchRange.h"
#include "TargetManager.h"

#include "EnemyStateNodeBase.h" 
#include "Slime_Plowling.h"

#include "Task_BodyBlow.h"
#include "Task_Wait.h"
#include "Slime_Attack.h"
#include "Slime_Chase.h"
#include "Slime_KnockBack.h"
#include "Slime_Pressed.h"
#include "Slime_Absorbed.h"
#include "GameTimer.h"
#include "PressData.h"
#include "Slime_Dyning.h"

#include "Utility_EnemyStator.h"

#include "AnimationMainStateMachine.h"
#include "AnimatorBase.h"
#include "Animator_NormalSlime.h"

#include "ChaseState_TargetSeek.h"
#include "ChaseState_AstarSeek.h"
#include "Task_MovePositions.h"

#include "ScalePop.h"

#include "ImpactMap.h"
#include "GraphAstar.h"

#include "GameManager.h"

namespace basecross {
	namespace Enemy {

		using namespace StateNode;

		//--------------------------------------------------------------------------------------
		/// パラメータ
		//--------------------------------------------------------------------------------------

		struct Stator_NormalSlime::Parametor
		{
			std::shared_ptr<Slime_Absorbed_Parametor> absorbedParamPtr;      //吸収されている状態
			std::shared_ptr<Slime_Attack::Parametor> absorbedAttackParamPtr; //吸収されている状態の攻撃パラメータ
			std::shared_ptr<Slime_Plowling::Parametor> plowlingParamPtr;     //徘徊
			std::shared_ptr<Slime_Chase::Parametor> chaseParamPtr;           //追従
			std::shared_ptr<Slime_KnockBack::Parametor> knockBackParamPtr;   //ノックバック
			std::shared_ptr<Slime_Attack::Parametor> attackParamPtr;         //攻撃
			std::shared_ptr<Slime_Pressed::Parametor> pressedParamPtr;       //潰される
			std::shared_ptr<Slime_Dyning::Parametor> deathParamPtr;          //死亡

			Parametor():
				absorbedParamPtr(new Slime_Absorbed::Parametor()),
				absorbedAttackParamPtr(new Slime_Attack::Parametor()),
				plowlingParamPtr(new Slime_Plowling::Parametor()),
				chaseParamPtr(new Slime_Chase::Parametor()),
				knockBackParamPtr(new Slime_KnockBack::Parametor()),
				attackParamPtr(new Slime_Attack::Parametor()),
				pressedParamPtr(new Slime_Pressed::Parametor()),
				deathParamPtr(new Slime_Dyning::Parametor())
			{}
		};

		//--------------------------------------------------------------------------------------
		/// 遷移条件メンバー
		//--------------------------------------------------------------------------------------

		Stator_NormalSlime_TransitionMember::Stator_NormalSlime_TransitionMember()
			:Stator_NormalSlime_TransitionMember(10.0f, 15.0f, 6.0f)
		{}

		Stator_NormalSlime_TransitionMember::Stator_NormalSlime_TransitionMember(
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
		bool Stator_NormalSlime::IsBodyBlowRange(const Stator_NormalSlime::TransitionMember& member) {
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
		bool ToChaseTrigger(const Stator_NormalSlime::TransitionMember& member) {
			return member.chaseTrigger.Get();
		}

		//ノックバックトリガー
		bool ToKnockBackTrigger(const Stator_NormalSlime::TransitionMember& member) {
			return member.knockBackTrigger.Get();
		}

		//--------------------------------------------------------------------------------------
		/// 通常スライムのステーター
		//--------------------------------------------------------------------------------------

		Stator_NormalSlime::Stator_NormalSlime(const std::shared_ptr<GameObject>& objPtr):
			Stator_NormalSlime(objPtr, TransitionMember())
		{}

		Stator_NormalSlime::Stator_NormalSlime(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member):
			SlimeStatorBase(objPtr, member),
			m_paramPtr(new Stator_NormalSlime::Parametor())
		{
			//ブラックボードの初期化
			auto& blackBoard = GetRefBlackBoardParam();
			blackBoard.deathParamPtr = m_paramPtr->deathParamPtr;
			blackBoard.attackParamPtr = m_paramPtr->attackParamPtr;
		}

		void Stator_NormalSlime::CreateNode() {
			auto enemy = GetGameObject()->GetComponent<EnemyBase>();
			auto animator = GetGameObject()->GetComponent<NormalSlime::Animator>(false);
			SettingParametor();  //パラメータの仮セッティング

			//Start(生成中)
			m_stateMachine->AddNode(State::Start, nullptr);

			//徘徊
			m_stateMachine->AddNode(State::Plowling, std::make_shared<StateNode::Slime_Plowling>(enemy, m_paramPtr->plowlingParamPtr));

			//追従
			m_stateMachine->AddNode(State::Chase, std::make_shared<StateNode::Slime_Chase>(enemy, m_paramPtr->chaseParamPtr));

			//ノックバック
			//ノックバックアニメーション
			m_paramPtr->knockBackParamPtr->startAnimationFunction = 
				[&, this]() { GetGameObject()->GetComponent<NormalSlime::Animator>(false)->ChangeState(NormalSlime::Animator::StateType::KnockBack); };
			//ノックバック終了時に呼び出すアニメーションを設定。
			m_paramPtr->knockBackParamPtr->waitParamPtr->exit = 
				[&, this]() { GetGameObject()->GetComponent<NormalSlime::Animator>(false)->ChangeState(NormalSlime::Animator::StateType::Idle); };
			m_stateMachine->AddNode(State::KnockBack, std::make_shared<StateNode::Slime_KnockBack>(enemy, m_paramPtr->knockBackParamPtr));

			//攻撃
			m_stateMachine->AddNode(State::Attack, std::make_shared<StateNode::Slime_Attack>(enemy, m_paramPtr->attackParamPtr));

			//Pressed
			//Pressed用のアニメーション設定
			const auto& pressedParamPtr = m_paramPtr->pressedParamPtr;
			pressedParamPtr->m_startAnimation = [&, this]() {  
				GetGameObject()->GetComponent<NormalSlime::Animator>(false)->ChangeState(NormalSlime::Animator::StateType::Pressed);
			};
			m_stateMachine->AddNode(State::Pressed, std::make_shared<Enemy::StateNode::Slime_Pressed>(enemy, m_paramPtr->pressedParamPtr));

			//死亡(Dyning)
			m_stateMachine->AddNode(State::Dyning, std::make_shared<Enemy::StateNode::Slime_Dyning>(enemy, m_paramPtr->deathParamPtr));

			//Death
			m_stateMachine->AddNode(State::Death, nullptr);

			//吸収された状態(基本的にUpdateを親依存)
			m_stateMachine->AddNode(State::Absorbed, std::make_shared<StateNode::Slime_Absorbed>(enemy, m_paramPtr->absorbedParamPtr));
			//吸収中の攻撃
			auto& absorbedAttackParamPtr = m_paramPtr->absorbedAttackParamPtr;
			*(absorbedAttackParamPtr) = *(m_paramPtr->attackParamPtr.get());
			absorbedAttackParamPtr->waitParamPtr->time = 0.01f;
			m_stateMachine->AddNode(State::AbsorbedAttack, std::make_shared<StateNode::Slime_Attack>(enemy, m_paramPtr->absorbedAttackParamPtr));

			//None
			m_stateMachine->AddNode(State::None, nullptr);
		}

		void Stator_NormalSlime::CreateEdge() {
			using namespace UtilityStator;

			//Start
			m_stateMachine->AddEdge(State::Start, State::Plowling,
				[&](const TransitionMember& member) { return GameManager::GetInstance()->GetCurrentState() == GameManager::State::Game; });

			//徘徊
			m_stateMachine->AddEdge(State::Plowling, State::Chase, 
				[&, this](const TransitionMember& member) { return Transition::InEyeRangePlayer(GetGameObject(), member.startChaseRange); });
			m_stateMachine->AddEdge(State::Plowling, State::KnockBack, &ToKnockBackTrigger);

			//Chase
			m_stateMachine->AddEdge(State::Chase, State::Plowling, 
				[&, this](const TransitionMember& member) { return Transition::LostTarget(GetGameObject(), member.maxChaseRange); });
			m_stateMachine->AddEdge(State::Chase, State::Attack, 
				[&, this](const TransitionMember& member) { return IsBodyBlowRange(member); });
			m_stateMachine->AddEdge(State::Chase, State::KnockBack, &ToKnockBackTrigger);

			//ノックバック
			m_stateMachine->AddEdge(State::KnockBack, State::KnockBack,
				[&, this](const TransitionMember& member) { return member.knockBackTrigger.Get(); });
			m_stateMachine->AddEdge(State::KnockBack, State::Plowling, &ToKnockBackTrigger);
			m_stateMachine->AddEdge(State::KnockBack, State::Plowling, 
				[&, this](const TransitionMember& member) { return true; }, true);

			//Attack
			m_stateMachine->AddEdge(State::Attack, State::Chase, &ToChaseTrigger);
			m_stateMachine->AddEdge(State::Attack, State::Chase, [&](const TransitionMember& member) { return true; }, true);
			m_stateMachine->AddEdge(State::Attack, State::KnockBack, &ToKnockBackTrigger);

			//Pressed
			m_stateMachine->AddEdge(State::Pressed, State::Dyning, 
				[](const TransitionMember& member) { return true; }, true);

			//Dyning
			m_stateMachine->AddEdge(State::Dyning, State::Death,
				[](const TransitionMember& member) { return true; }, true);

			//AbsorbedAttack
			m_stateMachine->AddEdge(State::AbsorbedAttack, State::Absorbed, [](const TransitionMember& member) { return true; }, true);
		}

		//--------------------------------------------------------------------------------------
		/// パラメータ設定
		//--------------------------------------------------------------------------------------

		void Stator_NormalSlime::SettingParametor() {
			SettingPlowlingParametor();
			SettingAttackParametor();
			SettingChaseParametor();
		}

		void Stator_NormalSlime::SettingPlowlingParametor() {
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
			moveParamPtr->targetNearRange = 6.0f;
			moveParamPtr->moveType = Task_ToTargetMove::MoveType::ArriveVelocity;
		}

		void Stator_NormalSlime::SettingChaseParametor() {
			auto& chaseParamPtr = m_paramPtr->chaseParamPtr;
			//探索時
			chaseParamPtr->astarSeekParamPtr->moveParamPtr->moveParamPtr->speed = 2.0f;
			//追従時
			chaseParamPtr->targetSeekParamPtr->seekParamPtr->speed = 5.0f;
		}

		void Stator_NormalSlime::SettingAttackParametor() {
			auto& attackParamPtr = m_paramPtr->attackParamPtr;
			attackParamPtr->waitParamPtr->time = 0.25f;
		}
	}
}

//endbasecross