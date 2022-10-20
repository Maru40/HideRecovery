/*!
@file AIPlayerBehaviorTree.cpp
@brief AIPlayerBehaviorTreeなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "AIPlayerBehaviorTree.h"

#include "Maruyama/Enemy/Behavior/BehaviorTree.h"
#include "Maruyama/Enemy/Behavior/Interface/I_Selecter.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Enemy/Component/BehaviorTree/Task/Task_SearchBall.h"

namespace basecross {
	namespace Enemy {

		AIPlayerBehaviorTree::AIPlayerBehaviorTree(const std::shared_ptr<GameObject>& objPtr) :
			BehaviorTreeComponentBase(objPtr)
		{}

		void AIPlayerBehaviorTree::CreateNode() {
			using namespace maru::Behavior;

			auto owner = GetGameObject()->GetComponent<EnemyBase>();

			//初回セレクター
			m_behaviorTree->AddSelecter(BehaviorType::FirstSelecter);

			//徘徊セレクター
			m_behaviorTree->AddSelecter(BehaviorType::PlowlingSelecter);
			//ボール探しタスク
			m_behaviorTree->AddTask(BehaviorType::SearchBallTask, std::make_shared<Task::SearchBall>(owner));

			//バトルセレクター
			m_behaviorTree->AddSelecter(BehaviorType::ButtleSelecter);
			//攻撃セレクター
			m_behaviorTree->AddSelecter(BehaviorType::AttackSelecter);


		}

		void AIPlayerBehaviorTree::CreateEdge() {
			using PriorityControllerBase = maru::Behavior::PriorityControllerBase;

			//初回セレクター
			m_behaviorTree->AddEdge(BehaviorType::FirstSelecter, BehaviorType::PlowlingSelecter, std::make_shared<PriorityControllerBase>(0.0f));
			m_behaviorTree->AddEdge(BehaviorType::FirstSelecter, BehaviorType::ButtleSelecter, std::make_shared<PriorityControllerBase>(1.0f));

			//徘徊セレクター
			m_behaviorTree->AddEdge(BehaviorType::PlowlingSelecter, BehaviorType::SearchBallTask, std::make_shared<PriorityControllerBase>(0.0f));

			//バトルセレクター
			m_behaviorTree->AddEdge(BehaviorType::ButtleSelecter, BehaviorType::AttackSelecter, std::make_shared<PriorityControllerBase>(0.0f));
		}

	}
}