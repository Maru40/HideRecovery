/*!
@file Stator_Hero.cpp
@brief Stator_Heroなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "StatorBase.h"
#include "EnemyStatorBase.h"
#include "Stator_Hero.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Enemy/StateMachine/EnemyMainStateMachine.h"

#include "Utility_EnemyStator.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"
#include "Maruyama/Utility/Timer/GameTimer.h"
#include "Maruyama/Enemy/StateMachine/EnemyStateNodeBase.h"
#include "Maruyama/Enemy/Component/Stator/StateNode/EnemyStateNode_Chase.h"
#include "Maruyama/Enemy/Component/Stator/StateNode/EnemyStateNode_Plowling.h"

#include "Maruyama/Utility/SingletonComponent/GameManager.h"

#include "TaskList.h"
#include "Task_Wait.h"
#include "Task_PlowlingMove.h"
#include "Task_ToTargetMove.h"

#include "Targeted.h"
#include "TargetManager.h"

#include "Maruyama/Interface/I_FactionMember.h"
#include "Maruyama/Enemy/AIDirector/FactionCoordinator.h"
#include "Maruyama/Enemy/AIDirector/PatrolCoordinator.h"

#include "Maruyama/Enemy/Component/EyeSearchRange.h"

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
			plowlingEyeRange(100.0f),
			attackRange(1.0f)
		{}

		//--------------------------------------------------------------------------------------
		/// 遷移条件
		//--------------------------------------------------------------------------------------

		bool Stator_Hero::IsInEyeRangeTarget(const TransitionMember& member) {
			auto targets = maru::Utility::FindComponents<Targeted>(GetStage());
			auto eye = GetGameObject()->GetComponent<EyeSearchRange>(false);
			if (!eye) {
				return false;
			}

			for (auto target : targets) {
				if (!target->GetGameObject()->IsActive()) {
					continue;
				}

				if (eye->IsInEyeRange(target->GetGameObject(), member.plowlingEyeRange)) {
					//ターゲット管理にターゲットを設定
					//if (auto& targetManager = GetGameObject()->GetComponent<TargetManager>(false)) {
					//	targetManager->SetTarget(target->GetGameObject());
					//}

					//ファクションに通知
					if (auto& factionMember = GetGameObject()->GetComponent<I_FactionMember>(false)) {
						auto faction = factionMember->GetAssignedFaction();
						auto tupleSpace = faction->GetTupleSpace();

						tupleSpace->Write<Tuple::FindTarget>(Tuple::FindTarget(GetGameObject(), target->GetGameObject()));
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
			moveParamPtr->moveType = Task::ToTargetMove::MoveType::ArriveVelocity;
		}

		void Stator_Hero::SettingChase() {

		}

	}
}