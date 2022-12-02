/*!
@file Stator_Hero.cpp
@brief Stator_Hero�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Stator/StatorBase.h"
#include "EnemyStatorBase.h"
#include "Stator_Hero.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Enemy/StateMachine/EnemyMainStateMachine.h"

#include "Utility_EnemyStator.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"
#include "Maruyama/Utility/Timer/GameTimer.h"
#include "Maruyama/Enemy/StateMachine/EnemyStateNodeBase.h"
#include "Maruyama/Enemy/Component/Stator/StateNode/EnemyStateNode_Chase.h"
#include "Maruyama/Enemy/Component/Stator/StateNode/EnemyStateNode_Plowling.h"

#include "Maruyama/Utility/SingletonComponent/GameManager.h"

#include "Maruyama/TaskList/TaskList.h"
#include "Maruyama/TaskList/CommonTasks/Task_Wait.h"
#include "Maruyama/TaskList/CommonTasks/Task_PlowlingMove.h"
#include "Maruyama/TaskList/CommonTasks/Task_ToTargetMove.h"

#include "Maruyama/Utility/Component/Targeted.h"
#include "Maruyama/Utility/Component/TargetManager.h"

#include "Maruyama/Interface/I_FactionMember.h"
#include "Maruyama/Enemy/AIDirector/FactionCoordinator.h"
#include "Maruyama/Enemy/AIDirector/PatrolCoordinator.h"

#include "Maruyama/Enemy/Component/EyeSearchRange.h"

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
			plowlingEyeRange(100.0f),
			attackRange(1.0f)
		{}

		//--------------------------------------------------------------------------------------
		/// �J�ڏ���
		//--------------------------------------------------------------------------------------

		bool Stator_Hero::IsInEyeRangeTarget(const TransitionMember& member) {
			auto targets = maru::Utility::FindComponents<Targeted>(GetStage());
			auto eye = GetGameObject()->GetComponent<EyeSearchRange>(false);
			if (!eye) {
				return false;
			}

			for (auto target : targets) {
				if (!target->GetGameObject()->IsActive()) {
					continue;
				}

				if (eye->IsInEyeRange(target->GetGameObject(), member.plowlingEyeRange)) {
					//�^�[�Q�b�g�Ǘ��Ƀ^�[�Q�b�g��ݒ�
					//if (auto& targetManager = GetGameObject()->GetComponent<TargetManager>(false)) {
					//	targetManager->SetTarget(target->GetGameObject());
					//}

					////�t�@�N�V�����ɒʒm
					//if (auto& factionMember = GetGameObject()->GetComponent<I_FactionMember>(false)) {
					//	auto faction = factionMember->GetAssignedFaction();
					//	auto tupleSpace = faction->GetTupleSpace();

					//	tupleSpace->Write<Tuple::FindTarget>(Tuple::FindTarget(GetGameObject(), target->GetGameObject()));
					//}

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
			//m_stateMachine->AddNode(State::Start, nullptr);

			////�p�j
			//m_stateMachine->AddNode(
			//	State::Plowling, std::make_shared<Enemy::StateNode::Plowling>(enemy, m_paramPtr->plowlingParamPtr));

			////�Ǐ]
			//m_stateMachine->AddNode(
			//	State::Chase, std::make_shared<Enemy::StateNode::Chase>(enemy, m_paramPtr->chaseParamPtr));
		}

		void Stator_Hero::CreateEdge() {
			using namespace UtilityStator;

			////Start
			//m_stateMachine->AddEdge(State::Start, State::Plowling,
			//	[&](const TransitionMember& member) { return GameManager::GetInstance()->GetCurrentState() == GameManager::State::Game; });

			////Plowling
			//m_stateMachine->AddEdge(State::Plowling, State::Chase, [&](const TransitionMember& member) { return IsInEyeRangeTarget(member); });

			////Chase
			//m_stateMachine->AddEdge(State::Chase, State::Plowling, 
			//	[&](const TransitionMember& member) { 
			//		//�^�[�Q�b�g�����݂��邩�ǂ���
			//		if (auto targetManager = GetGameObject()->GetComponent<TargetManager>(false)) {
			//			return !targetManager->HasTarget();
			//		}

			//		return false;
			//	}
			//);

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
			moveParamPtr->moveType = Task::ToTargetMove::MoveType::ArriveVelocity;
		}

		void Stator_Hero::SettingChase() {

		}

	}
}