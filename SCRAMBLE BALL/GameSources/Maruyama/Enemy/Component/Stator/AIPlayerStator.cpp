
/*!
@file AIPlayerStator.cpp
@brief AIPlayerStator�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "StatorBase.h"

#include "AIPlayerStator.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Enemy/StateMachine/EnemyMainStateMachine.h"

#include "Maruyama/Enemy/Component/Stator/StateNode/StateNode_HidePlacePatrol.h"
#include "Maruyama/Enemy/Component/Stator/StateNode/StateNode_Buttle.h"
#include "Maruyama/Enemy/Component/Stator/StateNode/StateNode_Dyning.h"
#include "Maruyama/Enemy/Component/Stator/StateNode/StateNode_Dead.h"
#include "Maruyama/Enemy/Component/Stator/StateNode/StateNode_Goal.h"

#include "Maruyama/Enemy/Component/SelfAstarNodeController.h"

#include "Maruyama/Enemy/Component/EyeSearchRange.h"
#include "Maruyama/Utility/ObserveIsInEyeTarget.h"

#include "Maruyama/Utility/SingletonComponent/GameManager.h"

#include "Maruyama/Interface/I_FactionMember.h"
#include "Maruyama/Interface/I_TeamMember.h"
#include "Maruyama/Enemy/AIDirector/EnemyAIDirector.h"
#include "Maruyama/Enemy/AIDirector/FactionCoordinator.h"
#include "Maruyama/Enemy/AIDirector/TupleSpace.h"

#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"

#include "Maruyama/Utility/Component/TargetManager.h"

#include "Maruyama/Enemy/AIDirector/CombatCoordinator.h"

#include "Watanabe/DebugClass/Debug.h"

#include "Patch/PlayerInputer.h"
#include "Watanabe/Component/PlayerStatus.h"
#include "Maruyama/Player/Component/PlayerDeader.h"

#include "Maruyama/Utility/Component/Targeted.h"
#include "Itabashi/ObjectMover.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		///	AIPlayerStator�̑J�ڏ��������o�[
		//--------------------------------------------------------------------------------------

		AIPlayerStator_TransitionMember::AIPlayerStator_TransitionMember():
			hidePatrolEeyRange(5.0f),
			buttleStartEyeRange(30.0f)
		{}

		//--------------------------------------------------------------------------------------
		///	�J�ڏ�����
		//--------------------------------------------------------------------------------------

		bool IsGameState(const AIPlayerStator::TransitionMember& member) {
			//�Q�[����Ԃ��ǂ����𔻒f
			bool isTransition = GameManager::GetInstance()->IsCurrentState(GameManager::State::Game);

			return isTransition;
		}

		bool AIPlayerStator::IsFindButtleTarget(const TransitionMember& member) {
			auto tupleSpace = m_tupler.lock()->GetTupleSpace();
			auto buttleTransition = tupleSpace->Take<Tuple::ButtleTransition>();

			//���b�Z�[�W���͂��Ă�����B
			if (buttleTransition && buttleTransition->GetValue() < member.buttleStartEyeRange) {
				auto targeted = buttleTransition->GetTarget()->GetComponent<Targeted>(false);
				if (targeted && !targeted->CanTarget()) {
					return false;
				}

				//�^�[�Q�b�g�̐ݒ�
				auto targetManager = m_targetManager.lock();
				auto target = buttleTransition->GetTarget();
				targetManager->SetTarget(target);

				return true;
			}

			return false;
		}

		bool AIPlayerStator::IsLostButtleTarget(const TransitionMember& member) {
			auto tupleSpace = m_tupler.lock()->GetTupleSpace();
			auto patrolTransition = tupleSpace->Take<Tuple::PatrolTransition>();

			if (patrolTransition) {
				auto tup = tupleSpace->Take<Tuple::PatrolTransition>();

				return true;
			}

			return false;
		}

		//--------------------------------------------------------------------------------------
		///	AIPlayerStator�{��
		//--------------------------------------------------------------------------------------

		AIPlayerStator::AIPlayerStator(const std::shared_ptr<GameObject>& objPtr):
			StatorBase(objPtr),
			m_eye(objPtr->GetComponent<EyeSearchRange>())
		{
			m_teamMember = objPtr->GetComponent<I_TeamMember>(false);
			m_factionMember = objPtr->GetComponent<I_FactionMember>(false);
			m_tupler = objPtr->GetComponent<Tuple::I_Tupler>(false);
			m_targetManager = objPtr->GetComponent<TargetManager>(false);
		}

		void AIPlayerStator::OnLateStart() {

		}

		void AIPlayerStator::CreateNode() {
			auto enemy = GetGameObject()->GetComponent<EnemyBase>();

			//None���
			m_stateMachine->AddNode(StateType::None, nullptr);

			//�B���ꏊ��T�����
			m_stateMachine->AddNode(StateType::HidePlacePatrol, std::make_shared<StateNode::HidePlacePatrol>(enemy));

			//�o�g��
			m_stateMachine->AddNode(StateType::Buttle, std::make_shared<StateNode::Buttle>(enemy));

			//�S�[����
			m_stateMachine->AddNode(StateType::Goal, std::make_shared<StateNode::Goal>(enemy));

			//���S��
			m_stateMachine->AddNode(StateType::Dyning, std::make_shared<StateNode::Dyning>(enemy));

			//���S
			m_stateMachine->AddNode(StateType::Dead, std::make_shared<StateNode::Dead>(enemy));
		}

		void AIPlayerStator::CreateEdge() {
			auto enemy = GetGameObject()->GetComponent<EnemyBase>();

			//None
			m_stateMachine->AddEdge(StateType::None, StateType::HidePlacePatrol, &IsGameState);
			//m_stateMachine->AddEdge(StateType::None, StateType::Buttle, &IsGameState);

			//�B���ꏊ��T��
			m_stateMachine->AddEdge(
				StateType::HidePlacePatrol, 
				StateType::Buttle, 
				[&](const TransitionMember& member) { return IsFindButtleTarget(member); }
			);

			//�o�g��
			m_stateMachine->AddEdge(
				StateType::Buttle,
				StateType::HidePlacePatrol,
				[&](const TransitionMember& member) { return IsLostButtleTarget(member); }
			);

			//�S�[��
			m_stateMachine->AddEdge(
				StateType::Goal,
				StateType::HidePlacePatrol,
				[](const TransitionMember& member) { return true; },
				true
			);
		}

		void AIPlayerStator::OnEnable() {
			ChangeState(StateType::None);

			if (auto objectMover = GetGameObject()->GetComponent<Operator::ObjectMover>(false)) {
				objectMover->SetDefaultForward(Vec3::Forward());
			}
		}
	}
}