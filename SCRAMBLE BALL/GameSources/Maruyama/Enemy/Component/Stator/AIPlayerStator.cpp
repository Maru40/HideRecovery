
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

#include "Maruyama/Utility/SingletonComponent/GameManager.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		///	�J�ڏ�����
		//--------------------------------------------------------------------------------------

		bool IsGameState(const AIPlayerStator::TransitionMember& member) {
			//�Q�[����Ԃ��ǂ����𔻒f
			return GameManager::GetInstance()->IsCurrentState(GameManager::State::Game);
		}

		//--------------------------------------------------------------------------------------
		///	AIPlayerStator�{��
		//--------------------------------------------------------------------------------------

		AIPlayerStator::AIPlayerStator(const std::shared_ptr<GameObject>& objPtr):
			StatorBase(objPtr)
		{}

		void AIPlayerStator::CreateNode() {
			auto enemy = GetGameObject()->GetComponent<EnemyBase>();

			//None���
			m_stateMachine->AddNode(StateType::None, nullptr);

			//�B���ꏊ��T�����
			m_stateMachine->AddNode(StateType::HidePlacePatrol, std::make_shared<StateNode::HidePlacePatrol>(enemy));

			//�U��

		}

		void AIPlayerStator::CreateEdge() {
			auto enemy = GetGameObject()->GetComponent<EnemyBase>();

			//None
			m_stateMachine->AddEdge(StateType::None, StateType::HidePlacePatrol, &IsGameState);

			//�B���ꏊ��T��

			//�U��
		}

	}
}