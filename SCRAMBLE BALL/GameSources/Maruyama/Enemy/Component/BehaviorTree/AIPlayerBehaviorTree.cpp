/*!
@file AIPlayerBehaviorTree.cpp
@brief AIPlayerBehaviorTreeなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "AIPlayerBehaviorTree.h"

#include "Maruyama/Enemy/Behavior/BehaviorTree.h"

#include "Maruyama/Enemy/Behavior/Interface/I_Selecter.h"

namespace basecross {
	namespace Enemy {

		AIPlayerBehaviorTree::AIPlayerBehaviorTree(const std::shared_ptr<GameObject>& objPtr) :
			BehaviorTreeComponentBase(objPtr)
		{}

		void AIPlayerBehaviorTree::CreateNode() {
			//初回セレクター
			m_behaviorTree->AddSelecter(BehaviorType::FirstSelecter);

			//バトルセレクター
			m_behaviorTree->AddSelecter(BehaviorType::ButtleSelecter);


		}

		void AIPlayerBehaviorTree::CreateEdge() {

		}

	}
}