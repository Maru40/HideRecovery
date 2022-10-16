/*!
@file AIPlayerBehaviorTree.cpp
@brief AIPlayerBehaviorTree�Ȃǎ���
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
			//����Z���N�^�[
			m_behaviorTree->AddSelecter(BehaviorType::FirstSelecter);

			//�o�g���Z���N�^�[
			m_behaviorTree->AddSelecter(BehaviorType::ButtleSelecter);


		}

		void AIPlayerBehaviorTree::CreateEdge() {

		}

	}
}