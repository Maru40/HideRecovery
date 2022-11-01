
/*!
@file AIPlayerStator.cpp
@brief AIPlayerStator�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "AIPlayerStator.h"

#include "Maruyama/Enemy/StateMachine/EnemyMainStateMachine.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

namespace basecross {
	namespace Enemy {

		AIPlayerStator::AIPlayerStator(const std::shared_ptr<GameObject>& objPtr):
			StatorBase(objPtr)
		{}

		void AIPlayerStator::CreateNode() {
			auto enemy = GetGameObject()->GetComponent<EnemyBase>();

			//None���
			m_stateMachine->AddNode(StateType::None, nullptr);

			//�B���ꏊ��T�����
			//m_stateMachine->AddEdge(StateType::HidePlacePatrol, );

			//�U��
		}

		void AIPlayerStator::CreateEdge() {
			auto enemy = GetGameObject()->GetComponent<EnemyBase>();

			//None

			//�B���ꏊ��T��

			//�U��
		}

	}
}