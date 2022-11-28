/*!
@file AIPlayerBehaviorTree.cpp
@brief AIPlayerBehaviorTree�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "AIPlayerBehaviorTree.h"

#include "Maruyama/Enemy/Behavior/BehaviorTree.h"
#include "Maruyama/Enemy/Behavior/Interface/I_Selecter.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Enemy/Behavior/Task/Task_SearchBall.h"

namespace basecross {
	namespace Enemy {

		AIPlayerBehaviorTree::AIPlayerBehaviorTree(const std::shared_ptr<GameObject>& objPtr) :
			BehaviorTreeComponentBase(objPtr)
		{}

		void AIPlayerBehaviorTree::CreateNode() {
			using namespace maru::Behavior;

			auto owner = GetGameObject()->GetComponent<EnemyBase>();

			//����Z���N�^�[
			m_behaviorTree->AddSelecter(BehaviorType::FirstSelecter);

			//�p�j�Z���N�^�[
			m_behaviorTree->AddSelecter(BehaviorType::PlowlingSelecter);
			//�{�[���T���^�X�N
			m_behaviorTree->AddTask(BehaviorType::SearchBallTask, std::make_shared<maru::Behavior::Task::SearchBall>(owner));

			//�o�g���Z���N�^�[
			m_behaviorTree->AddSelecter(BehaviorType::ButtleSelecter);
			//�U���Z���N�^�[
			m_behaviorTree->AddSelecter(BehaviorType::AttackSelecter);


		}

		void AIPlayerBehaviorTree::CreateEdge() {
			//using PriorityControllerBase = maru::Behavior::PriorityControllerBase;

			//����Z���N�^�[
			//m_behaviorTree->AddEdge(BehaviorType::FirstSelecter, BehaviorType::PlowlingSelecter, std::make_shared<PriorityControllerBase>(0.0f));
			//m_behaviorTree->AddEdge(BehaviorType::FirstSelecter, BehaviorType::ButtleSelecter, std::make_shared<PriorityControllerBase>(1.0f));

			////�p�j�Z���N�^�[
			//m_behaviorTree->AddEdge(BehaviorType::PlowlingSelecter, BehaviorType::SearchBallTask, std::make_shared<PriorityControllerBase>(0.0f));

			////�o�g���Z���N�^�[
			//m_behaviorTree->AddEdge(BehaviorType::ButtleSelecter, BehaviorType::AttackSelecter, std::make_shared<PriorityControllerBase>(0.0f));
		}

		void AIPlayerBehaviorTree::CreateDecorator() {
			//�p�j�f�R���[�^

		}

	}
}