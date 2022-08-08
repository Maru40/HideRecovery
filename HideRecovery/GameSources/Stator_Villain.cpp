/*!
@file Stator_Villain.cpp
@brief Stator_Villain�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "StatorBase.h"
#include "EnemyStatorBase.h"
#include "Stator_Villain.h"

#include "EnemyBase.h"
#include "EnemyMainStateMachine.h"

#include "Utility_EnemyStator.h"

#include "TimeHelper.h"
#include "GameTimer.h"
#include "EnemyStateNodeBase.h"
#include "EnemyStateNode_Plowling.h"

#include "GameManager.h"

#include "TaskList.h"
#include "Task_Wait.h"
#include "Task_PlowlingMove.h"
#include "Task_ToTargetMove.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �p�����[�^
		//--------------------------------------------------------------------------------------

		struct Stator_Villain::Parametor {
			std::shared_ptr<Enemy::StateNode::Plowling::Parametor> plowlingParamPtr;    //�p�j

			Parametor() :
				plowlingParamPtr(new Enemy::StateNode::Plowling::Parametor())
			{}
		};

		//--------------------------------------------------------------------------------------
		/// �J�ڏ��������o�[
		//--------------------------------------------------------------------------------------

		Stator_Villain_TransitionMember::Stator_Villain_TransitionMember() :
			attackRange(1.0f)
		{}

		//--------------------------------------------------------------------------------------
		/// �J�ڏ���
		//--------------------------------------------------------------------------------------



		//--------------------------------------------------------------------------------------
		/// �q�[���[�̃X�e�[�^�[
		//--------------------------------------------------------------------------------------

		Stator_Villain::Stator_Villain(const std::shared_ptr<GameObject>& objPtr)
			: Stator_Villain(objPtr, TransitionMember())
		{}

		Stator_Villain::Stator_Villain(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member)
			: EnemyStatorBase(objPtr, member), m_paramPtr(new Parametor())
		{
			//GetRefBlackBoardParam().deathParamPtr = m_paramPtr->deathParamPtr;
		}

		void Stator_Villain::CreateNode() {
			SettingParametor();
			auto enemy = GetGameObject()->GetComponent<EnemyBase>();

			//Start(������)
			m_stateMachine->AddNode(State::Start, nullptr);

			//�p�j
			m_stateMachine->AddNode(
				State::Plowling, std::make_shared<Enemy::StateNode::Plowling>(enemy, m_paramPtr->plowlingParamPtr));
		}

		void Stator_Villain::CreateEdge() {
			using namespace UtilityStator;

			//Start
			m_stateMachine->AddEdge(State::Start, State::Plowling,
				[&](const TransitionMember& member) { return GameManager::GetInstance()->GetCurrentState() == GameManager::State::Game; });

		}

		void Stator_Villain::SettingParametor() {
			SettingPlowling();
		}

		void Stator_Villain::SettingPlowling() {
			auto& plowlingParamPtr = m_paramPtr->plowlingParamPtr;

			plowlingParamPtr->waitParamPtr->time = 1.0f;

			//�ړ��p�����[�^
			auto& moveParamPtr = plowlingParamPtr->moveParamPtr->moveParamPtr;
			moveParamPtr->speed = 5.0f;
			moveParamPtr->targetNearRange = 6.0f;
			moveParamPtr->moveType = Task_ToTargetMove::MoveType::ArriveVelocity;
		}

	}
}