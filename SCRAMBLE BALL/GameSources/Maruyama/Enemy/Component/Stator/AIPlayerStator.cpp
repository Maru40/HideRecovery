
/*!
@file AIPlayerStator.cpp
@brief AIPlayerStator‚ÌƒNƒ‰ƒXÀ‘Ì
’S“–FŠÛR—TŠì
*/

#include "stdafx.h"
#include "Project.h"

#include "StatorBase.h"

#include "AIPlayerStator.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Enemy/StateMachine/EnemyMainStateMachine.h"

#include "Maruyama/Enemy/Component/Stator/StateNode/StateNode_HidePlacePatrol.h"

#include "Maruyama/Utility/SingletonComponent/GameManager.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		///	‘JˆÚğŒ®
		//--------------------------------------------------------------------------------------

		bool IsGameState(const AIPlayerStator::TransitionMember& member) {
			//ƒQ[ƒ€ó‘Ô‚©‚Ç‚¤‚©‚ğ”»’f
			return GameManager::GetInstance()->IsCurrentState(GameManager::State::Game);
		}

		//--------------------------------------------------------------------------------------
		///	AIPlayerStator–{‘Ì
		//--------------------------------------------------------------------------------------

		AIPlayerStator::AIPlayerStator(const std::shared_ptr<GameObject>& objPtr):
			StatorBase(objPtr)
		{}

		void AIPlayerStator::CreateNode() {
			auto enemy = GetGameObject()->GetComponent<EnemyBase>();

			//Noneó‘Ô
			m_stateMachine->AddNode(StateType::None, nullptr);

			//‰B‚µêŠ‚ğ’T‚·ó‘Ô
			m_stateMachine->AddNode(StateType::HidePlacePatrol, std::make_shared<StateNode::HidePlacePatrol>(enemy));

			//UŒ‚

		}

		void AIPlayerStator::CreateEdge() {
			auto enemy = GetGameObject()->GetComponent<EnemyBase>();

			//None
			m_stateMachine->AddEdge(StateType::None, StateType::HidePlacePatrol, &IsGameState);

			//‰B‚µêŠ‚ğ’T‚·

			//UŒ‚
		}

	}
}