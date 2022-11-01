/*!
@file HidePlacePatrolTree.cpp
@brief HidePlacePatrolTreeなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "HidePlacePatrolTree.h"

#include "Maruyama/Enemy/Behavior/BehaviorTree.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Enemy/Component/BehaviorTree/Task/Task_SearchBall.h"
#include "Maruyama/Enemy/Behavior/Interface/I_PriorityController.h"

namespace basecross {
	namespace Enemy {

		HidePlacePatrolTree::HidePlacePatrolTree(const std::shared_ptr<GameObject>& objPtr) :
			m_owner(objPtr),
			m_behaviorTree(new BehaviorTree())
		{}

		void HidePlacePatrolTree::OnCreate() {
			CreateNode();
			CreateEdge();
			CreateDecorator();
		}

		void HidePlacePatrolTree::OnUpdate() {
			m_behaviorTree->OnUpdate();
		}

		void HidePlacePatrolTree::CreateNode() {
			using namespace maru::Behavior;

			auto owner = GetOwner()->GetComponent<EnemyBase>();

			//初回セレクター
			m_behaviorTree->AddSelecter(BehaviorType::FirstSelecter);

			//ボール探すタスク
			m_behaviorTree->AddTask(
				BehaviorType::PatrolTask,
				std::make_shared<maru::Behavior::Task::SearchBall>(owner)
			);
		}

		void HidePlacePatrolTree::CreateEdge() {
			using PriorityControllerBase = maru::Behavior::PriorityControllerBase;

			//初回セレクター
			m_behaviorTree->AddEdge(BehaviorType::FirstSelecter, BehaviorType::PatrolTask, std::make_shared<PriorityControllerBase>(0.0f));
		}

		void HidePlacePatrolTree::CreateDecorator() {

		}


	}
}