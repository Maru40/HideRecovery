/*!
@file Stator_Hero.cpp
@brief Stator_Hero�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "StatorBase.h"
#include "EnemyStatorBase.h"
#include "Stator_Hero.h"

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

		struct Stator_Hero::Parametor {
			std::shared_ptr<Enemy::StateNode::Plowling::Parametor> plowlingParamPtr;    //�p�j

			Parametor():
				plowlingParamPtr(new Enemy::StateNode::Plowling::Parametor())
			{}
		};

		//--------------------------------------------------------------------------------------
		/// �J�ڏ��������o�[
		//--------------------------------------------------------------------------------------

		Stator_Hero_TransitionMember::Stator_Hero_TransitionMember() :
			attackRange(1.0f)
		{}

		//--------------------------------------------------------------------------------------
		/// �J�ڏ���
		//--------------------------------------------------------------------------------------



		//--------------------------------------------------------------------------------------
		/// �q�[���[�̃X�e�[�^�[
		//--------------------------------------------------------------------------------------

		Stator_Hero::Stator_Hero(const std::shared_ptr<GameObject>& objPtr)
			: Stator_Hero(objPtr, TransitionMember())
		{}

		Stator_Hero::Stator_Hero(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member)
			: EnemyStatorBase(objPtr, member), m_paramPtr(new Parametor())
		{
			//GetRefBlackBoardParam().deathParamPtr = m_paramPtr->deathParamPtr;
		}

		void Stator_Hero::CreateNode() {
			SettingParametor();
			auto enemy = GetGameObject()->GetComponent<EnemyBase>();

			//Start(������)
			m_stateMachine->AddNode(State::Start, nullptr);

			//�p�j
			m_stateMachine->AddNode(
				State::Plowling, std::make_shared<Enemy::StateNode::Plowling>(enemy, m_paramPtr->plowlingParamPtr));
		}

		void Stator_Hero::CreateEdge() {
			using namespace UtilityStator;

			//Start
			m_stateMachine->AddEdge(State::Start, State::Plowling,
				[&](const TransitionMember& member) { return GameManager::GetInstance()->GetCurrentState() == GameManager::State::Game; });

		}

		void Stator_Hero::SettingParametor() {
			SettingPlowling();
		}

		void Stator_Hero::SettingPlowling() {
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