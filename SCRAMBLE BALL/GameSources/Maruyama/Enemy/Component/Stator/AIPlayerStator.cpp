
/*!
@file AIPlayerStator.cpp
@brief AIPlayerStator‚ÌƒNƒ‰ƒXÀ‘Ì
’S“–FŠÛR—TŠì
*/

#include "stdafx.h"
#include "Project.h"

#include "AIPlayerStator.h"

#include "Maruyama/Enemy/StateMachine/EnemyMainStateMachine.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

namespace basecross {
	namespace Enemy {

		AIPlayerStator::AIPlayerStator(const std::shared_ptr<GameObject>& objPtr):
			StatorBase(objPtr)
		{}

		void AIPlayerStator::CreateNode() {
			auto enemy = GetGameObject()->GetComponent<EnemyBase>();

			//Noneó‘Ô
			m_stateMachine->AddNode(StateType::None, nullptr);

			//‰B‚µêŠ‚ğ’T‚·ó‘Ô
			//m_stateMachine->AddEdge(StateType::HidePlacePatrol, );

			//UŒ‚
		}

		void AIPlayerStator::CreateEdge() {
			auto enemy = GetGameObject()->GetComponent<EnemyBase>();

			//None

			//‰B‚µêŠ‚ğ’T‚·

			//UŒ‚
		}

	}
}