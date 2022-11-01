/*!
@file HidePlacePatrolTree.cpp
@brief HidePlacePatrolTreeなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "ButtleTree.h"

#include "Maruyama/Enemy/Behavior/BehaviorTree.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Enemy/Component/BehaviorTree/Task/Task_SearchBall.h"
#include "Maruyama/Enemy/Behavior/Interface/I_PriorityController.h"

namespace basecross {

	namespace maru {
		namespace Behavior {

			namespace SubBehavior {

				ButtleTree::ButtleTree(const std::shared_ptr<GameObject>& owner) :
					SubBehaviorTreeBase(owner)
				{}

				void ButtleTree::CreateNode() {
					//初期セレクター
					m_behaviorTree->AddSelecter(NodeType::FirstSelecter);

					//攻撃セレクター
					m_behaviorTree->AddSelecter(NodeType::AttackSelecter);

					//攻撃中の移動セレクター
					m_behaviorTree->AddSelecter(NodeType::AttackMoveSelecter);

					//近づくタスク
					//m_behaviorTree->AddTask(NodeType::NearSeekMoveTask,);
					
					//回り込みセレクター
					m_behaviorTree->AddSelecter(NodeType::WanparoundSelecter);

					////右から回り込むタスク
					//m_behaviorTree->AddTask(NodeType::RightSideMoveTask);

					////左から回り込むタスク
					//m_behaviorTree->AddTask(NodeType::LeftSideMoveTask);

					////裏取をするタスク
					//m_behaviorTree->AddTask(NodeType::BackSideMoveTask);


					//射撃攻撃タスク
					//m_behaviorTree->AddTask(NodeType::ShotTask);

					//退避セレクター
					m_behaviorTree->AddSelecter(NodeType::EvadeSelecter);

					//足止めセレクター
					m_behaviorTree->AddSelecter(NodeType::InducementStaySelecter);

					//足止め攻撃
					//m_behaviorTree->AddTask(NodeType::InducementAttack);

					//退避移動
					//m_behaviorTree->AddTask(NodeType::EvadeMoveTask);
				}

				void ButtleTree::CreateEdge() {
					//初期セレクター
					m_behaviorTree->AddEdge(NodeType::FirstSelecter, NodeType::AttackSelecter, (int)NodeType::AttackSelecter);
					m_behaviorTree->AddEdge(NodeType::FirstSelecter, NodeType::EvadeSelecter, (int)NodeType::EvadeSelecter);

					//攻撃セレクター
					m_behaviorTree->AddEdge(NodeType::AttackSelecter, NodeType::AttackMoveSelecter, (int)NodeType::AttackMoveSelecter);
					m_behaviorTree->AddEdge(NodeType::AttackSelecter, NodeType::ShotTask, (int)NodeType::ShotTask);

					//攻撃中の移動セレクター
					m_behaviorTree->AddEdge(NodeType::AttackMoveSelecter, NodeType::NearSeekMoveTask, (int)NodeType::NearSeekMoveTask);
					m_behaviorTree->AddEdge(NodeType::AttackMoveSelecter, NodeType::WanparoundSelecter, (int)NodeType::WanparoundSelecter);

					//回り込みセレクター
					m_behaviorTree->AddEdge(NodeType::WanparoundSelecter, NodeType::RightSideMoveTask, (int)NodeType::RightSideMoveTask);
				}

				void ButtleTree::CreateDecorator() {

				}

			}
		}
	}
}