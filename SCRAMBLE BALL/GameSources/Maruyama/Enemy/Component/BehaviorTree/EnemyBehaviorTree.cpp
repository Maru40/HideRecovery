/*!
@file EnemyBehaviorTree.cpp
@brief EnemyBehaviorTree�Ȃǎ���
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
			//����Z���N�^�[
			m_behaviorTree->AddSelecter(BehaviorType::FirstSelecter);

			//�o�g���Z���N�^�[
			m_behaviorTree->AddSelecter(BehaviorType::ButtleSelecter);


		}

		void EnemyBehaviorTree::CreateEdge() {

		}

	}
}