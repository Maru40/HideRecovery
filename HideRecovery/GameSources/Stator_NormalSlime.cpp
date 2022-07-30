
/*!
@file Stator_NormalSlime.cpp
@brief Stator_NormalSlime�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Trigger.h"

#include "EnemyMainStateMachine.h"
#include "StatorBase.h"
#include "SlimeStatorBase.h"
#include "Stator_NormalSlime.h"

#include "EnemyBase.h"
#include "EyeSearchRange.h"
#include "TargetManager.h"

#include "EnemyStateNodeBase.h" 
#include "Slime_Plowling.h"

#include "Task_BodyBlow.h"
#include "Task_Wait.h"
#include "Slime_Attack.h"
#include "Slime_Chase.h"
#include "Slime_KnockBack.h"
#include "Slime_Pressed.h"
#include "Slime_Absorbed.h"
#include "GameTimer.h"
#include "PressData.h"
#include "Slime_Dyning.h"

#include "Utility_EnemyStator.h"

#include "AnimationMainStateMachine.h"
#include "AnimatorBase.h"
#include "Animator_NormalSlime.h"

#include "ChaseState_TargetSeek.h"
#include "ChaseState_AstarSeek.h"
#include "Task_MovePositions.h"

#include "ScalePop.h"

#include "ImpactMap.h"
#include "GraphAstar.h"

#include "GameManager.h"

namespace basecross {
	namespace Enemy {

		using namespace StateNode;

		//--------------------------------------------------------------------------------------
		/// �p�����[�^
		//--------------------------------------------------------------------------------------

		struct Stator_NormalSlime::Parametor
		{
			std::shared_ptr<Slime_Absorbed_Parametor> absorbedParamPtr;      //�z������Ă�����
			std::shared_ptr<Slime_Attack::Parametor> absorbedAttackParamPtr; //�z������Ă����Ԃ̍U���p�����[�^
			std::shared_ptr<Slime_Plowling::Parametor> plowlingParamPtr;     //�p�j
			std::shared_ptr<Slime_Chase::Parametor> chaseParamPtr;           //�Ǐ]
			std::shared_ptr<Slime_KnockBack::Parametor> knockBackParamPtr;   //�m�b�N�o�b�N
			std::shared_ptr<Slime_Attack::Parametor> attackParamPtr;         //�U��
			std::shared_ptr<Slime_Pressed::Parametor> pressedParamPtr;       //�ׂ����
			std::shared_ptr<Slime_Dyning::Parametor> deathParamPtr;          //���S

			Parametor():
				absorbedParamPtr(new Slime_Absorbed::Parametor()),
				absorbedAttackParamPtr(new Slime_Attack::Parametor()),
				plowlingParamPtr(new Slime_Plowling::Parametor()),
				chaseParamPtr(new Slime_Chase::Parametor()),
				knockBackParamPtr(new Slime_KnockBack::Parametor()),
				attackParamPtr(new Slime_Attack::Parametor()),
				pressedParamPtr(new Slime_Pressed::Parametor()),
				deathParamPtr(new Slime_Dyning::Parametor())
			{}
		};

		//--------------------------------------------------------------------------------------
		/// �J�ڏ��������o�[
		//--------------------------------------------------------------------------------------

		Stator_NormalSlime_TransitionMember::Stator_NormalSlime_TransitionMember()
			:Stator_NormalSlime_TransitionMember(10.0f, 15.0f, 6.0f)
		{}

		Stator_NormalSlime_TransitionMember::Stator_NormalSlime_TransitionMember(
			const float& startChaseRange, const float& maxChaseRange,
			const float& bodyBlowRange
		) :
			startChaseRange(startChaseRange), maxChaseRange(maxChaseRange), 
			bodyBlowRange(bodyBlowRange)
		{}

		//--------------------------------------------------------------------------------------
		/// �J�ڏ���
		//--------------------------------------------------------------------------------------

		//�^�b�N���U���ɑJ�ڂ��邩�ǂ���
		bool Stator_NormalSlime::IsBodyBlowRange(const Stator_NormalSlime::TransitionMember& member) {
			auto eye = GetGameObject()->GetComponent<EyeSearchRange>(false);
			auto targetManager = GetGameObject()->GetComponent<TargetManager>(false);
			//�~�����R���|�[�l���g������������
			if (!eye || !targetManager) {
				DebugObject::AddString(L"Stator_Slime�ŁABodyBlow�ɑJ�ڂ��邽�߂̃R���|�[�l���g������܂���B");
				return false;
			}

			if (!targetManager->HasTarget()) {
				return false;
			}

			//�^�[�Q�b�g�����E���͈̔͂ɂ�����J�ڂ���
			return eye->IsInEyeRange(targetManager->GetTarget(), member.bodyBlowRange);
		}

		//�Ǐ]�g���K�[
		bool ToChaseTrigger(const Stator_NormalSlime::TransitionMember& member) {
			return member.chaseTrigger.Get();
		}

		//�m�b�N�o�b�N�g���K�[
		bool ToKnockBackTrigger(const Stator_NormalSlime::TransitionMember& member) {
			return member.knockBackTrigger.Get();
		}

		//--------------------------------------------------------------------------------------
		/// �ʏ�X���C���̃X�e�[�^�[
		//--------------------------------------------------------------------------------------

		Stator_NormalSlime::Stator_NormalSlime(const std::shared_ptr<GameObject>& objPtr):
			Stator_NormalSlime(objPtr, TransitionMember())
		{}

		Stator_NormalSlime::Stator_NormalSlime(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member):
			SlimeStatorBase(objPtr, member),
			m_paramPtr(new Stator_NormalSlime::Parametor())
		{
			//�u���b�N�{�[�h�̏�����
			auto& blackBoard = GetRefBlackBoardParam();
			blackBoard.deathParamPtr = m_paramPtr->deathParamPtr;
			blackBoard.attackParamPtr = m_paramPtr->attackParamPtr;
		}

		void Stator_NormalSlime::CreateNode() {
			auto enemy = GetGameObject()->GetComponent<EnemyBase>();
			auto animator = GetGameObject()->GetComponent<NormalSlime::Animator>(false);
			SettingParametor();  //�p�����[�^�̉��Z�b�e�B���O

			//Start(������)
			m_stateMachine->AddNode(State::Start, nullptr);

			//�p�j
			m_stateMachine->AddNode(State::Plowling, std::make_shared<StateNode::Slime_Plowling>(enemy, m_paramPtr->plowlingParamPtr));

			//�Ǐ]
			m_stateMachine->AddNode(State::Chase, std::make_shared<StateNode::Slime_Chase>(enemy, m_paramPtr->chaseParamPtr));

			//�m�b�N�o�b�N
			//�m�b�N�o�b�N�A�j���[�V����
			m_paramPtr->knockBackParamPtr->startAnimationFunction = 
				[&, this]() { GetGameObject()->GetComponent<NormalSlime::Animator>(false)->ChangeState(NormalSlime::Animator::StateType::KnockBack); };
			//�m�b�N�o�b�N�I�����ɌĂяo���A�j���[�V������ݒ�B
			m_paramPtr->knockBackParamPtr->waitParamPtr->exit = 
				[&, this]() { GetGameObject()->GetComponent<NormalSlime::Animator>(false)->ChangeState(NormalSlime::Animator::StateType::Idle); };
			m_stateMachine->AddNode(State::KnockBack, std::make_shared<StateNode::Slime_KnockBack>(enemy, m_paramPtr->knockBackParamPtr));

			//�U��
			m_stateMachine->AddNode(State::Attack, std::make_shared<StateNode::Slime_Attack>(enemy, m_paramPtr->attackParamPtr));

			//Pressed
			//Pressed�p�̃A�j���[�V�����ݒ�
			const auto& pressedParamPtr = m_paramPtr->pressedParamPtr;
			pressedParamPtr->m_startAnimation = [&, this]() {  
				GetGameObject()->GetComponent<NormalSlime::Animator>(false)->ChangeState(NormalSlime::Animator::StateType::Pressed);
			};
			m_stateMachine->AddNode(State::Pressed, std::make_shared<Enemy::StateNode::Slime_Pressed>(enemy, m_paramPtr->pressedParamPtr));

			//���S(Dyning)
			m_stateMachine->AddNode(State::Dyning, std::make_shared<Enemy::StateNode::Slime_Dyning>(enemy, m_paramPtr->deathParamPtr));

			//Death
			m_stateMachine->AddNode(State::Death, nullptr);

			//�z�����ꂽ���(��{�I��Update��e�ˑ�)
			m_stateMachine->AddNode(State::Absorbed, std::make_shared<StateNode::Slime_Absorbed>(enemy, m_paramPtr->absorbedParamPtr));
			//�z�����̍U��
			auto& absorbedAttackParamPtr = m_paramPtr->absorbedAttackParamPtr;
			*(absorbedAttackParamPtr) = *(m_paramPtr->attackParamPtr.get());
			absorbedAttackParamPtr->waitParamPtr->time = 0.01f;
			m_stateMachine->AddNode(State::AbsorbedAttack, std::make_shared<StateNode::Slime_Attack>(enemy, m_paramPtr->absorbedAttackParamPtr));

			//None
			m_stateMachine->AddNode(State::None, nullptr);
		}

		void Stator_NormalSlime::CreateEdge() {
			using namespace UtilityStator;

			//Start
			m_stateMachine->AddEdge(State::Start, State::Plowling,
				[&](const TransitionMember& member) { return GameManager::GetInstance()->GetCurrentState() == GameManager::State::Game; });

			//�p�j
			m_stateMachine->AddEdge(State::Plowling, State::Chase, 
				[&, this](const TransitionMember& member) { return Transition::InEyeRangePlayer(GetGameObject(), member.startChaseRange); });
			m_stateMachine->AddEdge(State::Plowling, State::KnockBack, &ToKnockBackTrigger);

			//Chase
			m_stateMachine->AddEdge(State::Chase, State::Plowling, 
				[&, this](const TransitionMember& member) { return Transition::LostTarget(GetGameObject(), member.maxChaseRange); });
			m_stateMachine->AddEdge(State::Chase, State::Attack, 
				[&, this](const TransitionMember& member) { return IsBodyBlowRange(member); });
			m_stateMachine->AddEdge(State::Chase, State::KnockBack, &ToKnockBackTrigger);

			//�m�b�N�o�b�N
			m_stateMachine->AddEdge(State::KnockBack, State::KnockBack,
				[&, this](const TransitionMember& member) { return member.knockBackTrigger.Get(); });
			m_stateMachine->AddEdge(State::KnockBack, State::Plowling, &ToKnockBackTrigger);
			m_stateMachine->AddEdge(State::KnockBack, State::Plowling, 
				[&, this](const TransitionMember& member) { return true; }, true);

			//Attack
			m_stateMachine->AddEdge(State::Attack, State::Chase, &ToChaseTrigger);
			m_stateMachine->AddEdge(State::Attack, State::Chase, [&](const TransitionMember& member) { return true; }, true);
			m_stateMachine->AddEdge(State::Attack, State::KnockBack, &ToKnockBackTrigger);

			//Pressed
			m_stateMachine->AddEdge(State::Pressed, State::Dyning, 
				[](const TransitionMember& member) { return true; }, true);

			//Dyning
			m_stateMachine->AddEdge(State::Dyning, State::Death,
				[](const TransitionMember& member) { return true; }, true);

			//AbsorbedAttack
			m_stateMachine->AddEdge(State::AbsorbedAttack, State::Absorbed, [](const TransitionMember& member) { return true; }, true);
		}

		//--------------------------------------------------------------------------------------
		/// �p�����[�^�ݒ�
		//--------------------------------------------------------------------------------------

		void Stator_NormalSlime::SettingParametor() {
			SettingPlowlingParametor();
			SettingAttackParametor();
			SettingChaseParametor();
		}

		void Stator_NormalSlime::SettingPlowlingParametor() {
			auto& plowlingParamPtr = m_paramPtr->plowlingParamPtr;

			plowlingParamPtr->positions = {
				Vec3(+5.0f, +0.5f, +3.0f),
				Vec3(-5.0f, +0.5f, +3.0f),
				Vec3(+5.0f, +0.5f, -3.0f)
			};

			plowlingParamPtr->waitParamPtr->time = 1.0f;

			//�ړ��p�����[�^
			auto& moveParamPtr = plowlingParamPtr->moveParamPtr->moveParamPtr;
			moveParamPtr->speed = 5.0f;
			moveParamPtr->targetNearRange = 6.0f;
			moveParamPtr->moveType = Task_ToTargetMove::MoveType::ArriveVelocity;
		}

		void Stator_NormalSlime::SettingChaseParametor() {
			auto& chaseParamPtr = m_paramPtr->chaseParamPtr;
			//�T����
			chaseParamPtr->astarSeekParamPtr->moveParamPtr->moveParamPtr->speed = 2.0f;
			//�Ǐ]��
			chaseParamPtr->targetSeekParamPtr->seekParamPtr->speed = 5.0f;
		}

		void Stator_NormalSlime::SettingAttackParametor() {
			auto& attackParamPtr = m_paramPtr->attackParamPtr;
			attackParamPtr->waitParamPtr->time = 0.25f;
		}
	}
}

//endbasecross