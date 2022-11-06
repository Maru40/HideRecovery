/*!
@file HidePlacePatrolTree.cpp
@brief HidePlacePatrolTreeなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "ButtleTree.h"

#include "Maruyama/Enemy/Behavior/BehaviorTree.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Enemy/Behavior/Task/BehaviorTask_Shot.h"
#include "Maruyama/Enemy/Behavior/Task/NearSeekMove.h"

#include "Maruyama/Enemy/Behavior/Interface/I_PriorityController.h"

namespace basecross {

	namespace maru {
		namespace Behavior {

			namespace SubBehavior {

				ButtleTree::ButtleTree(const std::shared_ptr<GameObject>& owner) :
					SubBehaviorTreeBase(owner)
				{}

				void ButtleTree::CreateNode() {
					auto enemy = GetOwner()->GetComponent<Enemy::EnemyBase>();

					//初期セレクター
					m_behaviorTree->AddSelecter(NodeType::FirstSelecter);

					//攻撃セレクター
					m_behaviorTree->AddSelecter(NodeType::AttackSelecter);

					//攻撃中の移動セレクター
					m_behaviorTree->AddSelecter(NodeType::AttackMoveSelecter);

					//近づくセレクター
					m_behaviorTree->AddSelecter(NodeType::NearMoveSelecter);

					//直線的に近づくセレクター
					m_behaviorTree->AddTask<Task::NearSeekMove>(NodeType::NearSeekMoveTask, enemy);

					//m_behaviorTree->AddTask(NodeType::NearAstarMoveTask);
					
					//回り込みセレクター
					m_behaviorTree->AddSelecter(NodeType::WanparoundSelecter);

					////右から回り込むタスク
					//m_behaviorTree->AddTask(NodeType::RightSideMoveTask);

					////左から回り込むタスク
					//m_behaviorTree->AddTask(NodeType::LeftSideMoveTask);

					////裏取をするタスク
					//m_behaviorTree->AddTask(NodeType::BackSideMoveTask);


					//射撃攻撃タスク
					m_behaviorTree->AddTask<Task::Shot>(NodeType::ShotTask, enemy);

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

					CreateAttackEdge();
					CreateEvadeEdge();
				}

				void ButtleTree::CreateAttackEdge() {
					//攻撃セレクター
					m_behaviorTree->AddEdge(NodeType::AttackSelecter, NodeType::AttackMoveSelecter, (int)NodeType::AttackMoveSelecter);
					m_behaviorTree->AddEdge(NodeType::AttackSelecter, NodeType::ShotTask, 0.0f);

					//攻撃中の移動セレクター
					m_behaviorTree->AddEdge(NodeType::AttackMoveSelecter, NodeType::NearMoveSelecter, (int)NodeType::NearMoveSelecter);
					m_behaviorTree->AddEdge(NodeType::AttackMoveSelecter, NodeType::WanparoundSelecter, (int)NodeType::WanparoundSelecter);

					//近づくときのセレクター
					m_behaviorTree->AddEdge(NodeType::NearMoveSelecter, NodeType::NearSeekMoveTask, (int)NodeType::NearSeekMoveTask);
					//m_behaviorTree->AddEdge(NodeType::NearMoveSelecter, NodeType::NearAstarMoveTask, (int)NodeType::NearAstarMoveTask);

					//回り込みセレクター
					//m_behaviorTree->AddEdge(NodeType::WanparoundSelecter, NodeType::RightSideMoveTask, (int)NodeType::RightSideMoveTask);
					//m_behaviorTree->AddEdge(NodeType::WanparoundSelecter, NodeType::LeftSideMoveTask, (int)NodeType::LeftSideMoveTask);
					//m_behaviorTree->AddEdge(NodeType::WanparoundSelecter, NodeType::BackSideMoveTask, (int)NodeType::BackSideMoveTask);
				}

				void ButtleTree::CreateEvadeEdge() {
					//逃亡セレクター
					m_behaviorTree->AddEdge(NodeType::EvadeSelecter, NodeType::InducementStaySelecter, (int)NodeType::InducementStaySelecter);
					//m_behaviorTree->AddEdge(NodeType::EvadeSelecter, NodeType::EvadeMoveTask, (int)NodeType::EvadeMoveTask);

					////足止めセレクター
					//m_behaviorTree->AddEdge(NodeType::InducementStaySelecter, NodeType::InducementAttack, (int)NodeType::InducementAttack);
				}

				void ButtleTree::CreateDecorator() {
					//Shotにデコレータ追加
					
				}

			}
		}
	}
}