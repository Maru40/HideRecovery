/*!
@file Stator_Hero.cpp
@brief Stator_Heroなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "StatorBase.h"
#include "EnemyStatorBase.h"
#include "Stator_Hero.h"

#include "EnemyBase.h"
#include "EnemyMainStateMachine.h"

#include "Utility_EnemyStator.h"

#include "TimeHelper.h"
#include "GameTimer.h"
#include "EnemyStateNodeBase.h"
#include "EnemyStateNode_Plowling.h"
#include "EnemyStateNode_Chase.h"

#include "GameManager.h"

#include "TaskList.h"
#include "Task_Wait.h"
#include "Task_PlowlingMove.h"
#include "Task_ToTargetMove.h"

#include "Targeted.h"
#include "TargetManager.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// パラメータ
		//--------------------------------------------------------------------------------------

		struct Stator_Hero::Parametor {
			std::shared_ptr<Enemy::StateNode::Plowling::Parametor> plowlingParamPtr;    //徘徊
			std::shared_ptr<Enemy::StateNode::Chase::Parametor> chaseParamPtr;			//追従

			Parametor():
				plowlingParamPtr(new Enemy::StateNode::Plowling::Parametor()),
				chaseParamPtr(new Enemy::StateNode::Chase::Parametor())
			{}
		};

		//--------------------------------------------------------------------------------------
		/// 遷移条件メンバー
		//--------------------------------------------------------------------------------------

		Stator_Hero_TransitionMember::Stator_Hero_TransitionMember() :
			plowlingEyeRange(10.0f),
			attackRange(1.0f)
		{}

		//--------------------------------------------------------------------------------------
		/// 遷移条件
		//--------------------------------------------------------------------------------------

		bool Stator_Hero::IsInEyeRangeTarget(const Stator_Hero_TransitionMember& member) {
			//auto hideItems 
			auto targets = maru::Utility::FindComponents<Targeted>();

			for (auto target : targets) {
				if (!target->GetGameObject()->IsActive()) {
					continue;
				}

				auto toTargetVec = maru::Utility::CalcuToTargetVec(GetGameObject(), target->GetGameObject());
				if (toTargetVec.length() < member.plowlingEyeRange) {
					//ターゲット管理にターゲットを設定
					if (auto& targetManager = GetGameObject()->GetComponent<TargetManager>(false)) {
						targetManager->SetTarget(target->GetGameObject());
					}

					return true;
				}
			}

			return false;
		}

		//--------------------------------------------------------------------------------------
		/// ヒーローのステーター
		//--------------------------------------------------------------------------------------

		Stator_Hero::Stator_Hero(const std::shared_ptr<GameObject>& objPtr)
			: Stator_Hero(objPtr, TransitionMember())
		{}

		Stator_Hero::Stator_Hero(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member)
			: EnemyStatorBase(objPtr, member), m_paramPtr(new Parametor())
		{
			//GetRefBlackBoardParam().deathParamPtr = m_paramPtr->deathParamPtr;
		}

		void Stator_Hero::CreateNode() {
			SettingParametor();
			auto enemy = GetGameObject()->GetComponent<EnemyBase>();

			//Start(生成中)
			m_stateMachine->AddNode(State::Start, nullptr);

			//徘徊
			m_stateMachine->AddNode(
				State::Plowling, std::make_shared<Enemy::StateNode::Plowling>(enemy, m_paramPtr->plowlingParamPtr));

			//追従
			m_stateMachine->AddNode(
				State::Chase, std::make_shared<Enemy::StateNode::Chase>(enemy, m_paramPtr->chaseParamPtr));
		}

		void Stator_Hero::CreateEdge() {
			using namespace UtilityStator;

			//Start
			m_stateMachine->AddEdge(State::Start, State::Plowling,
				[&](const TransitionMember& member) { return GameManager::GetInstance()->GetCurrentState() == GameManager::State::Game; });

			//Plowling
			m_stateMachine->AddEdge(State::Plowling, State::Chase, [&](const TransitionMember& member) { return IsInEyeRangeTarget(member); });

			//Chase
			m_stateMachine->AddEdge(State::Chase, State::Plowling, 
				[&](const TransitionMember& member) { 
					//ターゲットが存在するかどうか
					if (auto targetManager = GetGameObject()->GetComponent<TargetManager>(false)) {
						return !targetManager->HasTarget();
					}

					return false;
				}
			);

		}

		void Stator_Hero::SettingParametor() {
			SettingPlowling();
			SettingChase();
		}

		void Stator_Hero::SettingPlowling() {
			auto& plowlingParamPtr = m_paramPtr->plowlingParamPtr;

			plowlingParamPtr->waitParamPtr->time = 1.0f;

			//移動パラメータ
			auto& moveParamPtr = plowlingParamPtr->moveParamPtr->moveParamPtr;
			moveParamPtr->speed = 5.0f;
			moveParamPtr->targetNearRange = 6.0f;
			moveParamPtr->moveType = Task_ToTargetMove::MoveType::ArriveVelocity;
		}

		void Stator_Hero::SettingChase() {

		}

	}
}