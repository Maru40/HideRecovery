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
#include "EnemyStateNode_Chase.h"

#include "GameManager.h"

#include "TaskList.h"
#include "Task_Wait.h"
#include "Task_PlowlingMove.h"
#include "Task_ToTargetMove.h"

#include "Targeted.h"
#include "TargetManager.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �p�����[�^
		//--------------------------------------------------------------------------------------

		struct Stator_Hero::Parametor {
			std::shared_ptr<Enemy::StateNode::Plowling::Parametor> plowlingParamPtr;    //�p�j
			std::shared_ptr<Enemy::StateNode::Chase::Parametor> chaseParamPtr;			//�Ǐ]

			Parametor():
				plowlingParamPtr(new Enemy::StateNode::Plowling::Parametor()),
				chaseParamPtr(new Enemy::StateNode::Chase::Parametor())
			{}
		};

		//--------------------------------------------------------------------------------------
		/// �J�ڏ��������o�[
		//--------------------------------------------------------------------------------------

		Stator_Hero_TransitionMember::Stator_Hero_TransitionMember() :
			plowlingEyeRange(10.0f),
			attackRange(1.0f)
		{}

		//--------------------------------------------------------------------------------------
		/// �J�ڏ���
		//--------------------------------------------------------------------------------------

		bool Stator_Hero::IsInEyeRangeTarget(const Stator_Hero_TransitionMember& member) {
			//auto hideItems 
			auto targets = maru::Utility::FindComponents<Targeted>();

			for (auto target : targets) {
				if (!target->GetGameObject()->IsActive()) {
					continue;
				}

				auto toTargetVec = maru::Utility::CalcuToTargetVec(GetGameObject(), target->GetGameObject());
				if (toTargetVec.length() < member.plowlingEyeRange) {
					//�^�[�Q�b�g�Ǘ��Ƀ^�[�Q�b�g��ݒ�
					if (auto& targetManager = GetGameObject()->GetComponent<TargetManager>(false)) {
						targetManager->SetTarget(target->GetGameObject());
					}

					return true;
				}
			}

			return false;
		}

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

			//�Ǐ]
			m_stateMachine->AddNode(
				State::Chase, std::make_shared<Enemy::StateNode::Chase>(enemy, m_paramPtr->chaseParamPtr));
		}

		void Stator_Hero::CreateEdge() {
			using namespace UtilityStator;

			//Start
			m_stateMachine->AddEdge(State::Start, State::Plowling,
				[&](const TransitionMember& member) { return GameManager::GetInstance()->GetCurrentState() == GameManager::State::Game; });

			//Plowling
			m_stateMachine->AddEdge(State::Plowling, State::Chase, [&](const TransitionMember& member) { return IsInEyeRangeTarget(member); });

			//Chase
			m_stateMachine->AddEdge(State::Chase, State::Plowling, 
				[&](const TransitionMember& member) { 
					//�^�[�Q�b�g�����݂��邩�ǂ���
					if (auto targetManager = GetGameObject()->GetComponent<TargetManager>(false)) {
						return !targetManager->HasTarget();
					}

					return false;
				}
			);

		}

		void Stator_Hero::SettingParametor() {
			SettingPlowling();
			SettingChase();
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

		void Stator_Hero::SettingChase() {

		}

	}
}