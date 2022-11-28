/*!
@file Stator_Villain.cpp
@brief Stator_Villainなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "StatorBase.h"
#include "EnemyStatorBase.h"
#include "Stator_Villain.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Enemy/StateMachine/EnemyMainStateMachine.h"

#include "Utility_EnemyStator.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"
#include "Maruyama/Utility/Timer/GameTimer.h"
#include "Maruyama/Enemy/StateMachine/EnemyStateNodeBase.h"
#include "Maruyama/Enemy/Component/Stator/StateNode/EnemyStateNode_Plowling.h"

#include "Maruyama/Utility/SingletonComponent/GameManager.h"

#include "Maruyama/TaskList/TaskList.h"
#include "Maruyama/TaskList/CommonTasks/Task_Wait.h"
#include "Maruyama/TaskList/CommonTasks/Task_PlowlingMove.h"
#include "Maruyama/TaskList/CommonTasks/Task_ToTargetMove.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// パラメータ
		//--------------------------------------------------------------------------------------

		struct Stator_Villain::Parametor {
			std::shared_ptr<Enemy::StateNode::Plowling::Parametor> plowlingParamPtr;    //徘徊

			Parametor() :
				plowlingParamPtr(new Enemy::StateNode::Plowling::Parametor())
			{}
		};

		//--------------------------------------------------------------------------------------
		/// 遷移条件メンバー
		//--------------------------------------------------------------------------------------

		Stator_Villain_TransitionMember::Stator_Villain_TransitionMember() :
			plowlingEyeRange(10.0f),
			attackRange(1.0f)
		{}

		//--------------------------------------------------------------------------------------
		/// 遷移条件
		//--------------------------------------------------------------------------------------



		//--------------------------------------------------------------------------------------
		/// ヒーローのステーター
		//--------------------------------------------------------------------------------------

		Stator_Villain::Stator_Villain(const std::shared_ptr<GameObject>& objPtr)
			: Stator_Villain(objPtr, TransitionMember())
		{}

		Stator_Villain::Stator_Villain(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member)
			: EnemyStatorBase(objPtr, member), m_paramPtr(new Parametor())
		{
			//GetRefBlackBoardParam().deathParamPtr = m_paramPtr->deathParamPtr;
		}

		void Stator_Villain::CreateNode() {
			SettingParametor();
			auto enemy = GetGameObject()->GetComponent<EnemyBase>();

			////Start(生成中)
			//m_stateMachine->AddNode(State::Start, nullptr);

			////徘徊
			//m_stateMachine->AddNode(
			//	State::Plowling, std::make_shared<Enemy::StateNode::Plowling>(enemy, m_paramPtr->plowlingParamPtr));
		}

		void Stator_Villain::CreateEdge() {
			using namespace UtilityStator;

			////Start
			//m_stateMachine->AddEdge(State::Start, State::Plowling,
			//	[&](const TransitionMember& member) { return GameManager::GetInstance()->GetCurrentState() == GameManager::State::Game; });

		}

		void Stator_Villain::SettingParametor() {
			SettingPlowling();
		}

		void Stator_Villain::SettingPlowling() {
			auto& plowlingParamPtr = m_paramPtr->plowlingParamPtr;

			plowlingParamPtr->waitParamPtr->time = 1.0f;

			//移動パラメータ
			auto& moveParamPtr = plowlingParamPtr->moveParamPtr->moveParamPtr;
			moveParamPtr->speed = 5.0f;
			moveParamPtr->targetNearRange = 6.0f;
			moveParamPtr->moveType = Task::ToTargetMove::MoveType::ArriveVelocity;
		}

	}
}