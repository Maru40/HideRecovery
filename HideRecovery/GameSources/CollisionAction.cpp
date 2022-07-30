/*!
@file CollisionAction.cpp
@brief CollisionActioné¿ëÃ
íSìñÅFä€éRóTäÏ
*/

#include "stdafx.h"
#include "Project.h"

#include "CollisionAction.h"

namespace basecross {

	namespace maru {

		CollisionAction::CollisionAction(const std::shared_ptr<GameObject>& objPtr)
			:Component(objPtr)
		{}

		void CollisionAction::OnCollisionEnter(const CollisionPair& pair) {
			m_collisionAction.enter.Invoke(pair);
			m_triggerAction.enter.Invoke(pair);
		}

		void CollisionAction::OnCollisionExcute(const CollisionPair& pair) {
			m_collisionAction.excute.Invoke(pair);
			m_triggerAction.excute.Invoke(pair);
		}

		void CollisionAction::OnCollisionExit(const CollisionPair& pair) {
			m_collisionAction.exit.Invoke(pair);
			m_triggerAction.exit.Invoke(pair);
		}
	}
}

//endbasecross