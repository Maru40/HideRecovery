/*!
@file Stator_BigSlime.cpp
@brief Stator_BigSlime�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "StatorBase.h"
#include "SlimeStatorBase.h"
#include "Stator_BigSlime.h"

#include "EnemyBase.h"
#include "EnemyMainStateMachine.h"

#include "TimeHelper.h"
#include "GameTimer.h"
#include "TargetManager.h"
#include "TaskList.h"
#include "Task_Wait.h"
#include "Task_BodyBlow.h"
#include "EnemyStateNodeBase.h"
#include "Slime_Plowling.h"
#include "Slime_KnockBack.h"
#include "PressData.h"
#include "Slime_Pressed.h"
#include "Slime_Dyning.h"
#include "Slime_Absorbed.h"
#include "ChaseState_AstarSeek.h"
#include "ChaseState_TargetSeek.h"
#include "Task_MovePositions.h"
#include "Task_AroundEyeCheck.h"
#include "Slime_Chase.h"
#include "Slime_Attack.h"
#include "BigSlime_Attack.h"

#include "EvadeSlime_Evade.h"

#include "AnimationMainStateMachine.h"
#include "AnimatorBase.h"
#include "Animator_BigSlime.h"

#include "Utility_EnemyStator.h"

#include "BigSlime_Dyning.h"

#include "EyeSearchRange.h"
#include "GameManager.h"

namespace basecross {
	namespace Enemy {

		namespace BigSlime {

			using namespace StateNode;

			//--------------------------------------------------------------------------------------
			/// �p�����[�^
			//--------------------------------------------------------------------------------------
			struct Stator::Parametor
			{
				std::shared_ptr<Slime_Absorbed_Parametor> absorbedParamPtr;     //�z������Ă�����
				std::shared_ptr<Slime_Plowling::Parametor> plowlingParamPtr;    //�p�j
				std::shared_ptr<Slime_Chase::Parametor> chaseParamPtr;          //�Ǐ]
				std::shared_ptr<Slime_KnockBack::Parametor> knockBackParamPtr;  //�m�b�N�o�b�N
				std::shared_ptr<BigSlime_Attack::Parametor> attackParamPtr;     //�U��
				std::shared_ptr<Slime_Pressed::Parametor> pressedParamPtr;      //�ׂ����
				std::shared_ptr<Slime_Dyning::Parametor> dyningParamPtr;        //���S

				Parametor() :
					absorbedParamPtr(new Slime_Absorbed_Parametor()),
					plowlingParamPtr(new Slime_Plowling::Parametor()),
					chaseParamPtr(new Slime_Chase::Parametor()),
					knockBackParamPtr(new Slime_KnockBack::Parametor()),
					attackParamPtr(new BigSlime_Attack::Parametor()),
					pressedParamPtr(new Slime_Pressed::Parametor()),
					dyningParamPtr(new Slime_Dyning::Parametor())
				{}
			};

			//--------------------------------------------------------------------------------------
			/// �J�ڏ��������o
			//--------------------------------------------------------------------------------------

			Stator_TransitionMemeber::Stator_TransitionMemeber()
				:Stator_TransitionMemeber(10.0f, 15.0f, 6.0f)
			{}

			Stator_TransitionMemeber::Stator_TransitionMemeber(
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
			bool Stator::IsBodyBlowRange(const Stator::TransitionMember& member) {
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
			bool ToChaseTrigger(const Stator::TransitionMember& member) {
				return member.chaseTrigger.Get();
			}

			bool ToKnockBackTrigger(const Stator::TransitionMember& member) {
				return member.knockBackTrigger.Get();
			}

			//--------------------------------------------------------------------------------------
			/// ����X���C���̃X�e�[�^�[�{��
			//--------------------------------------------------------------------------------------

			Stator::Stator(const std::shared_ptr<GameObject>& owner):
				Stator(owner, TransitionMember())
			{ }

			Stator::Stator(const std::shared_ptr<GameObject>& owner, const TransitionMember& member):
				SlimeStatorBase(owner, member), m_paramPtr(new Parametor())
			{
				GetRefBlackBoardParam().dyningParamPtr = m_paramPtr->dyningParamPtr;
			}

			void Stator::CreateNode() {
				auto enemy = GetGameObject()->GetComponent<EnemyBase>();
				auto animator = GetGameObject()->GetComponent<BigSlime::Animator>(false);
				SettingParametor();

				//Start(������)
				m_stateMachine->AddNode(State::Start, nullptr);

				//�p�j
				m_stateMachine->AddNode(State::Plowling, std::make_shared<StateNode::Slime_Plowling>(enemy, m_paramPtr->plowlingParamPtr));

				//�Ǐ]
				m_stateMachine->AddNode(State::Chase, std::make_shared<StateNode::Slime_Chase>(enemy, m_paramPtr->chaseParamPtr));

				//�m�b�N�o�b�N
				m_stateMachine->AddNode(State::KnockBack, std::make_shared<StateNode::Slime_KnockBack>(enemy, m_paramPtr->knockBackParamPtr));

				//�U��
				m_stateMachine->AddNode(State::Attack, std::make_shared<StateNode::BigSlime_Attack>(enemy, m_paramPtr->attackParamPtr));  //�V�K�쐬

				//�ׂ����
				m_stateMachine->AddNode(State::Pressed, std::make_shared<StateNode::Slime_Pressed>(enemy, m_paramPtr->pressedParamPtr));

				//���S
				m_stateMachine->AddNode(State::Dyning, std::make_shared<StateNode::Slime_Dyning>(enemy, m_paramPtr->dyningParamPtr));  //�V�K����

				//Death
				m_stateMachine->AddNode(State::Death, nullptr);

				//None
				m_stateMachine->AddNode(State::None, nullptr);
			}

			void Stator::CreateEdge() {
				using namespace UtilityStator;

				//Start
				m_stateMachine->AddEdge(State::Start, State::Plowling,
					[&](const TransitionMember& member) { return GameManager::GetInstance()->GetCurrentState() == GameManager::State::Game; });

				//�p�j
				m_stateMachine->AddEdge(State::Plowling, State::Chase, 
					[&, this](const TransitionMember& member) { return Transition::InEyeRangePlayer(GetGameObject(), member.startChaseRange); });
				m_stateMachine->AddEdge(State::Plowling, State::KnockBack, &ToKnockBackTrigger);

				//�Ǐ]
				m_stateMachine->AddEdge(State::Chase, State::Plowling,
					[&, this](const TransitionMember& member) { return Transition::LostTarget(GetGameObject(), member.maxChaseRange); });
				m_stateMachine->AddEdge(State::Chase, State::Attack,
					[&, this](const TransitionMember& member) { return IsBodyBlowRange(member); });
				m_stateMachine->AddEdge(State::Chase, State::KnockBack, &ToKnockBackTrigger);

				//�m�b�N�o�b�N
				m_stateMachine->AddEdge(State::KnockBack, State::KnockBack,
					[&, this](const TransitionMember& member) { return member.knockBackTrigger.Get(); });
				m_stateMachine->AddEdge(State::KnockBack, State::Plowling, &ToKnockBackTrigger);
				m_stateMachine->AddEdge(State::KnockBack, State::Plowling, [](const TransitionMember& member) { return true; }, true);

				//�U��
				m_stateMachine->AddEdge(State::Attack, State::Chase, &ToChaseTrigger);
				m_stateMachine->AddEdge(State::Attack, State::KnockBack, &ToKnockBackTrigger);
				m_stateMachine->AddEdge(State::Attack, State::Chase, [](const TransitionMember& member) { return true; }, true);

				//�ׂ����
				m_stateMachine->AddEdge(State::Pressed, State::Dyning,
					[](const TransitionMember& member) { return true; }, true);

				//���S
				m_stateMachine->AddEdge(State::Dyning, State::Death,
					[](const TransitionMember& member) { return true; }, true);
			}

			//--------------------------------------------------------------------------------------
			/// �p�����[�^�Z�b�e�B���O
			//--------------------------------------------------------------------------------------

			void Stator::SettingParametor() {
				SettingPlowling();
				SettingChase();
			}

			void Stator::SettingPlowling() {
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
				moveParamPtr->targetNearRange = 2.0f;
				moveParamPtr->moveType = Task_ToTargetMove::MoveType::ArriveVelocity;
			}

			void Stator::SettingChase() {
				auto& chaseParamPtr = m_paramPtr->chaseParamPtr;
				//�T����
				chaseParamPtr->astarSeekParamPtr->moveParamPtr->moveParamPtr->speed = 2.0f;
				//�Ǐ]��
				chaseParamPtr->targetSeekParamPtr->seekParamPtr->speed = 5.0f;
			}

		}
	}
}