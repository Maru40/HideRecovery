/*!
@file EnemyBehaviorTree.cpp
@brief EnemyBehaviorTree‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyBehaviorTree.h"

#include "Maruyama/Enemy/Behavior/BehaviorTree.h"

#include "Maruyama/Enemy/Behavior/Interface/I_Selecter.h"

namespace basecross {
	namespace Enemy {

		EnemyBehaviorTree::EnemyBehaviorTree(const std::shared_ptr<GameObject>& objPtr) :
			BehaviorTreeComponentBase(objPtr)
		{}

		void EnemyBehaviorTree::CreateNode() {
			m_behaviorTree->AddSelecter(BehaviorType::FirstSelecter);

		}

		void EnemyBehaviorTree::CreateEdge() {

		}

	}
}