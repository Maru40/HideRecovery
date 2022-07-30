/*!
@file Stator_ShootSlime.cpp
@brief Stator_ShootSlimeのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyMainStateMachine.h"
#include "StatorBase.h"
#include "SlimeStatorBase.h"
#include "Stator_ShootSlime.h"

#include "EnemyBase.h"

#include "EnemyStateNodeBase.h"
#include "Slime_Plowling.h"
#include "Slime_Chase.h"

#include "TaskList.h"
#include "GameTimer.h"
#include "Task_Wait.h"
#include "ShootManager.h"
#include "Task_Preliminary_Shoot.h"
#include "Task_Shoot.h"
#include "ShootSlime_Attack.h"

#include "PressData.h"
#include "Slime_KnockBack.h"
#include "Slime_Pressed.h"
#include "Slime_Dyning.h"
#include "Slime_Absorbed.h"

#include "Utility_EnemyStator.h"

#include "Task_Wait.h"

#include "AnimationMainStateMachine.h"
#include "AnimatorBase.h"
#include "Animator_ShootSlime.h"

#include "GameManager.h"

namespace basecross {
	namespace Enemy {

		namespace ShootSlime {

			//--------------------------------------------------------------------------------------
			/// パラメータ
			//--------------------------------------------------------------------------------------

			struct Stator_ShootSlime::Parametor {
				std::shared_ptr<Enemy::StateNode::Slime_Absorbed::Parametor> absorbedParamPtr;    //吸収
				std::shared_ptr<Enemy::StateNode::Slime_Plowling::Parametor> plowlingParamPtr;    //徘徊
				std::shared_ptr<StateNode::Attack::Parametor> attackParamPtr;                     //攻撃
				std::shared_ptr<Enemy::StateNode::Slime_Chase::Parametor> chaseParamPtr;          //追従
				std::shared_ptr<Enemy::StateNode::Slime_KnockBack::Parametor> knockBackParamPtr;  //ノックバックパラメータ
				std::shared_ptr<Enemy::StateNode::Slime_Pressed::Parametor> pressedParamPtr;      //潰される
				std::shared_ptr<Enemy::StateNode::Slime_Dyning::Parametor> deathParamPtr;         //死亡

				Parametor() :
					absorbedParamPtr(new Enemy::StateNode::Slime_Absorbed::Parametor()),
					plowlingParamPtr(new Enemy::StateNode::Slime_Plowling::Parametor()),
					attackParamPtr(new StateNode::Attack::Parametor()),
					chaseParamPtr(new Enemy::StateNode::Slime_Chase::Parametor()),
					knockBackParamPtr(new Enemy::StateNode::Slime_KnockBack::Parametor()),
					pressedParamPtr(new Enemy::StateNode::Slime_Pressed::Parametor()),
					deathParamPtr(new Enemy::StateNode::Slime_Dyning::Parametor())
				{}
			};

			//--------------------------------------------------------------------------------------
			/// 遷移条件メンバー
			//--------------------------------------------------------------------------------------

			Stator_ShootSlime_TransitionMember::Stator_ShootSlime_TransitionMember() :
				Stator_ShootSlime_TransitionMember(10.0f, 15.0f, 30.0f)
			{}

			Stator_ShootSlime_TransitionMember::Stator_ShootSlime_TransitionMember(
				const float& startChaseRange,
				const float& maxChaseRange,
				const float& attackStartRange
			) :
				startChaseRange(startChaseRange),
				maxChaseRange(maxChaseRange),
				attackStartRange(attackStartRange)
			{}

			//--------------------------------------------------------------------------------------
			/// 遷移条件
			//--------------------------------------------------------------------------------------

			bool ToKnockBackTrigger(const Stator_ShootSlime::TransitionMember& member) {
				return member.knockBackTrigger.Get();
			}

			//--------------------------------------------------------------------------------------
			/// ショットスライムのステーター
			//--------------------------------------------------------------------------------------

			Stator_ShootSlime::Stator_ShootSlime(const std::shared_ptr<GameObject>& objPtr)
				: Stator_ShootSlime(objPtr, TransitionMember())
			{}

			Stator_ShootSlime::Stator_ShootSlime(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member)
				: SlimeStatorBase(objPtr, member), m_paramPtr(new Parametor())
			{
				GetRefBlackBoardParam().deathParamPtr = m_paramPtr->deathParamPtr;
			}

			void Stator_ShootSlime::CreateNode() {
				auto enemy = GetGameObject()->GetComponent<EnemyBase>();
				SettingParametor();  //パラメータの仮セッティング

				//Start(生成中)
				m_stateMachine->AddNode(State::Start, nullptr);

				//徘徊
				m_stateMachine->AddNode(State::Plowling, std::make_shared<Enemy::StateNode::Slime_Plowling>(enemy, m_paramPtr->plowlingParamPtr));

				//追従
				m_stateMachine->AddNode(State::Chase, std::make_shared<Enemy::StateNode::Slime_Chase>(enemy, m_paramPtr->chaseParamPtr));

				//攻撃
				m_stateMachine->AddNode(State::Attack, std::make_shared<StateNode::Attack>(enemy, m_paramPtr->attackParamPtr));

				//ノックバック
				m_stateMachine->AddNode(State::KnockBack, std::make_shared<Enemy::StateNode::Slime_KnockBack>(enemy, m_paramPtr->knockBackParamPtr));

				//Pressed
				//Pressed用のアニメーション設定
				const auto& pressedParamPtr = m_paramPtr->pressedParamPtr;
				pressedParamPtr->m_startAnimation = [&, this]() {
					GetGameObject()->GetComponent<ShootSlime::Animator>(false)->ChangeState(ShootSlime::Animator::StateType::Pressed);
				};
				m_stateMachine->AddNode(State::Pressed, std::make_shared<Enemy::StateNode::Slime_Pressed>(enemy, m_paramPtr->pressedParamPtr));

				//死亡(Dyning)
				m_stateMachine->AddNode(State::Dyning, std::make_shared<Enemy::StateNode::Slime_Dyning>(enemy, m_paramPtr->deathParamPtr));

				//吸収中
				m_stateMachine->AddNode(State::Absorbed, std::make_shared<Enemy::StateNode::Slime_Absorbed>(enemy, m_paramPtr->absorbedParamPtr));

				//Death
				m_stateMachine->AddNode(State::Death, nullptr);

				//None
				m_stateMachine->AddNode(State::None, nullptr);
			}

			void Stator_ShootSlime::CreateEdge() {
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
					[&, this](const TransitionMember& member) { return Transition::InEyeRangeTarget(GetGameObject(), member.attackStartRange); });
				m_stateMachine->AddEdge(State::Chase, State::KnockBack, &ToKnockBackTrigger);

				//Attack
				m_stateMachine->AddEdge(State::Attack, State::Chase, [&](const TransitionMember& member) { return true; }, true);
				m_stateMachine->AddEdge(State::Attack, State::KnockBack, &ToKnockBackTrigger);

				//ノックバック
				m_stateMachine->AddEdge(State::KnockBack, State::KnockBack, &ToKnockBackTrigger);
				m_stateMachine->AddEdge(State::KnockBack, State::Plowling,
					[&, this](const TransitionMember& member) { return true; }, true);

				//Pressed
				m_stateMachine->AddEdge(State::Pressed, State::Dyning,
					[](const TransitionMember& member) { return true; }, true);

				//Dyning
				m_stateMachine->AddEdge(State::Dyning, State::Death,
					[](const TransitionMember& member) { return true; }, true);
			}

			void Stator_ShootSlime::SettingParametor() {
				SettingPlowlingParametor();
				SettingAttackParametor();
			}

			void Stator_ShootSlime::SettingPlowlingParametor() {
				auto& plowlingParamPtr = m_paramPtr->plowlingParamPtr;
				plowlingParamPtr->positions = {
					Vec3(+5.0f, +0.5f, +8.0f),
					Vec3(-5.0f, +0.5f, +8.0f),
				};
				plowlingParamPtr->waitParamPtr->time = 1.0f;

				//移動パラメータ
				auto& moveParamPtr = plowlingParamPtr->moveParamPtr->moveParamPtr;
				moveParamPtr->speed = 7.0f;
				moveParamPtr->targetNearRange = 7.0f;
				moveParamPtr->moveType = Task_ToTargetMove::MoveType::ArriveVelocity;
			}

			void Stator_ShootSlime::SettingAttackParametor() {
				auto& attackParamPtr = m_paramPtr->attackParamPtr;
				attackParamPtr->shootParamPtr->speed = 15.0f;
			}

		}
	}
}