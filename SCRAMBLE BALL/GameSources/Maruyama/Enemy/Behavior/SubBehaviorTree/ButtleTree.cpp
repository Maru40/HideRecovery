/*!
@file HidePlacePatrolTree.cpp
@brief HidePlacePatrolTree�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "ButtleTree.h"

#include "Maruyama/Enemy/Behavior/BehaviorTree.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Enemy/Behavior/Task/Task_SearchBall.h"
#include "Maruyama/Enemy/Behavior/Interface/I_PriorityController.h"

namespace basecross {

	namespace maru {
		namespace Behavior {

			namespace SubBehavior {

				ButtleTree::ButtleTree(const std::shared_ptr<GameObject>& owner) :
					SubBehaviorTreeBase(owner)
				{}

				void ButtleTree::CreateNode() {
					//�����Z���N�^�[
					m_behaviorTree->AddSelecter(NodeType::FirstSelecter);

					//�U���Z���N�^�[
					m_behaviorTree->AddSelecter(NodeType::AttackSelecter);

					//�U�����̈ړ��Z���N�^�[
					m_behaviorTree->AddSelecter(NodeType::AttackMoveSelecter);

					//�߂Â��^�X�N
					//m_behaviorTree->AddTask(NodeType::NearSeekMoveTask,);
					
					//��荞�݃Z���N�^�[
					m_behaviorTree->AddSelecter(NodeType::WanparoundSelecter);

					////�E�����荞�ރ^�X�N
					//m_behaviorTree->AddTask(NodeType::RightSideMoveTask);

					////�������荞�ރ^�X�N
					//m_behaviorTree->AddTask(NodeType::LeftSideMoveTask);

					////���������^�X�N
					//m_behaviorTree->AddTask(NodeType::BackSideMoveTask);


					//�ˌ��U���^�X�N
					//m_behaviorTree->AddTask(NodeType::ShotTask);

					//�ޔ��Z���N�^�[
					m_behaviorTree->AddSelecter(NodeType::EvadeSelecter);

					//���~�߃Z���N�^�[
					m_behaviorTree->AddSelecter(NodeType::InducementStaySelecter);

					//���~�ߍU��
					//m_behaviorTree->AddTask(NodeType::InducementAttack);

					//�ޔ��ړ�
					//m_behaviorTree->AddTask(NodeType::EvadeMoveTask);
				}

				void ButtleTree::CreateEdge() {
					//�����Z���N�^�[
					m_behaviorTree->AddEdge(NodeType::FirstSelecter, NodeType::AttackSelecter, (int)NodeType::AttackSelecter);
					m_behaviorTree->AddEdge(NodeType::FirstSelecter, NodeType::EvadeSelecter, (int)NodeType::EvadeSelecter);

					CreateAttackEdge();
					CreateEvadeEdge();
				}

				void ButtleTree::CreateAttackEdge() {
					//�U���Z���N�^�[
					m_behaviorTree->AddEdge(NodeType::AttackSelecter, NodeType::AttackMoveSelecter, (int)NodeType::AttackMoveSelecter);
					m_behaviorTree->AddEdge(NodeType::AttackSelecter, NodeType::ShotTask, (int)NodeType::ShotTask);

					//�U�����̈ړ��Z���N�^�[
					m_behaviorTree->AddEdge(NodeType::AttackMoveSelecter, NodeType::NearSeekMoveTask, (int)NodeType::NearSeekMoveTask);
					m_behaviorTree->AddEdge(NodeType::AttackMoveSelecter, NodeType::WanparoundSelecter, (int)NodeType::WanparoundSelecter);

					//��荞�݃Z���N�^�[
					m_behaviorTree->AddEdge(NodeType::WanparoundSelecter, NodeType::RightSideMoveTask, (int)NodeType::RightSideMoveTask);
					m_behaviorTree->AddEdge(NodeType::WanparoundSelecter, NodeType::LeftSideMoveTask, (int)NodeType::LeftSideMoveTask);
					m_behaviorTree->AddEdge(NodeType::WanparoundSelecter, NodeType::BackSideMoveTask, (int)NodeType::BackSideMoveTask);
				}

				void ButtleTree::CreateEvadeEdge() {
					//���S�Z���N�^�[
					m_behaviorTree->AddEdge(NodeType::EvadeSelecter, NodeType::InducementStaySelecter, (int)NodeType::InducementStaySelecter);
					m_behaviorTree->AddEdge(NodeType::EvadeSelecter, NodeType::EvadeMoveTask, (int)NodeType::EvadeMoveTask);

					//���~�߃Z���N�^�[
					m_behaviorTree->AddEdge(NodeType::InducementStaySelecter, NodeType::InducementAttack, (int)NodeType::InducementAttack);
				}

				void ButtleTree::CreateDecorator() {

				}

			}
		}
	}
}