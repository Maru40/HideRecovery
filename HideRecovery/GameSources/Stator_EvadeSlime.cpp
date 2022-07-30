/*!
@file Stator_EvadeSlime.cpp
@brief Stator_EvadeSlime�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "StatorBase.h"
#include "SlimeStatorBase.h"
#include "Stator_EvadeSlime.h"

#include "EnemyBase.h"
#include "EnemyMainStateMachine.h"

#include "TimeHelper.h"
#include "GameTimer.h"
#include "TargetManager.h"
#include "TaskList.h"
#include "Task_Wait.h"
#include "EnemyStateNodeBase.h"
#include "Slime_Plowling.h"
#include "Slime_KnockBack.h"
#include "PressData.h"
#include "Slime_Pressed.h"
#include "Slime_Dyning.h"

#include "EvadeSlime_Evade.h"

#include "AnimationMainStateMachine.h"
#include "AnimatorBase.h"
#include "Animator_EvadeSlime.h"

#include "Utility_EnemyStator.h"

namespace basecross {
	namespace Enemy {

		namespace EvadeSlime {

			//--------------------------------------------------------------------------------------
			/// �p�����[�^
			//--------------------------------------------------------------------------------------

			struct Stator_EvadeSlime::Parametor {
				std::shared_ptr<Enemy::StateNode::Slime_Plowling::Parametor> plowlingParamPtr;    //�p�j
				std::shared_ptr<Enemy::EvadeSlime::StateNode::Evade_Parametor> evadeParamPtr;     //������
				std::shared_ptr<Enemy::StateNode::Slime_KnockBack_Parametor> knockBackParamPtr;   //�m�b�N�o�b�N
				std::shared_ptr<Enemy::StateNode::Slime_Pressed::Parametor> pressedParamPtr;      //�ׂ����
				std::shared_ptr<Enemy::StateNode::Slime_Dyning::Parametor> deathParamPtr;         //���S

				Parametor() :
					plowlingParamPtr(new Enemy::StateNode::Slime_Plowling::Parametor()),
					evadeParamPtr(new Enemy::EvadeSlime::StateNode::Evade_Parametor()),
					knockBackParamPtr(new Enemy::StateNode::Slime_KnockBack::Parametor()),
					pressedParamPtr(new Enemy::StateNode::Slime_Pressed::Parametor()),
					deathParamPtr(new Enemy::StateNode::Slime_Dyning::Parametor())
				{}
			};

			//--------------------------------------------------------------------------------------
			/// �J�ڏ��������o
			//--------------------------------------------------------------------------------------

			Stator_EvadeSlime_TransitionMember::Stator_EvadeSlime_TransitionMember()
				:Stator_EvadeSlime_TransitionMember(6.0f, 20.0f)
			{}

			Stator_EvadeSlime_TransitionMember::Stator_EvadeSlime_TransitionMember(
				const float& startEvadeRange, const float& endEvadeRange
			) :
				startEvadeRange(startEvadeRange), endEvadeRange(endEvadeRange)
			{}

			//--------------------------------------------------------------------------------------
			/// ������X���C���̃X�e�[�^�[�{��
			//--------------------------------------------------------------------------------------

			Stator_EvadeSlime::Stator_EvadeSlime(const std::shared_ptr<GameObject>& objPtr)
				: Stator_EvadeSlime(objPtr, TransitionMember())
			{}

			Stator_EvadeSlime::Stator_EvadeSlime(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member)
				: SlimeStatorBase(objPtr, member), m_paramPtr(new Parametor())
			{
				GetRefBlackBoardParam().deathParamPtr = m_paramPtr->deathParamPtr;
			}

			void Stator_EvadeSlime::CreateNode() {
				auto enemy = GetGameObject()->GetComponent<EnemyBase>();
				SettingParametor();  //�p�����[�^�̉��Z�b�e�B���O

				//�p�j
				m_stateMachine->AddNode(State::Plowling, std::make_shared<Enemy::StateNode::Slime_Plowling>(enemy, m_paramPtr->plowlingParamPtr));

				//�Ǐ]
				m_stateMachine->AddNode(State::Evade, std::make_shared<Enemy::EvadeSlime::StateNode::Evade>(enemy, m_paramPtr->evadeParamPtr));

				//�m�b�N�o�b�N
				//�m�b�N�o�b�N�A�j���[�V����
				m_paramPtr->knockBackParamPtr->startAnimationFunction =
					[&, this]() { GetGameObject()->GetComponent<EvadeSlime::Animator>(false)->ChangeState(EvadeSlime::Animator::StateType::KnockBack); };
				//�m�b�N�o�b�N�I�����ɌĂяo���A�j���[�V������ݒ�B
				m_paramPtr->knockBackParamPtr->waitParamPtr->exit =
					[&, this]() { GetGameObject()->GetComponent<EvadeSlime::Animator>(false)->ChangeState(EvadeSlime::Animator::StateType::Idle); };
				m_stateMachine->AddNode(State::KnockBack, std::make_shared<Enemy::StateNode::Slime_KnockBack>(enemy, m_paramPtr->knockBackParamPtr));

				//Pressed
				//Pressed�p�̃A�j���[�V�����ݒ�
				const auto& pressedParamPtr = m_paramPtr->pressedParamPtr;
				pressedParamPtr->m_startAnimation = [&, this]() {
					GetGameObject()->GetComponent<EvadeSlime::Animator>(false)->ChangeState(EvadeSlime::Animator::StateType::Pressed);
				};
				m_stateMachine->AddNode(State::Pressed, std::make_shared<Enemy::StateNode::Slime_Pressed>(enemy, m_paramPtr->pressedParamPtr));

				//���S(Dyning)
				m_stateMachine->AddNode(State::Dyning, std::make_shared<Enemy::StateNode::Slime_Dyning>(enemy, m_paramPtr->deathParamPtr));

				//Death
				m_stateMachine->AddNode(State::Death, nullptr);
			}

			void Stator_EvadeSlime::CreateEdge() {
				using namespace UtilityStator;

				//�p�j
				m_stateMachine->AddEdge(State::Plowling, State::Evade,
					[&, this](const TransitionMember& member) { return Transition::InEyeRangePlayer(GetGameObject(), member.startEvadeRange); });
				m_stateMachine->AddEdge(State::Plowling, State::KnockBack,
					[&, this](const TransitionMember& member) { return member.knockBackTrigger.Get(); });

				//Evade
				m_stateMachine->AddEdge(State::Evade, State::Plowling,
					[&, this](const TransitionMember& member) { return Transition::LostTarget(GetGameObject(), member.endEvadeRange); });
				m_stateMachine->AddEdge(State::Evade, State::KnockBack,
					[&, this](const TransitionMember& member) { return member.knockBackTrigger.Get(); });

				//�m�b�N�o�b�N
				m_stateMachine->AddEdge(State::KnockBack, State::KnockBack,
					[&, this](const TransitionMember& member) { return member.knockBackTrigger.Get(); });
				m_stateMachine->AddEdge(State::KnockBack, State::Plowling,
					[&, this](const TransitionMember& member) { return true; }, true);

				//Pressed
				m_stateMachine->AddEdge(State::Pressed, State::Dyning,
					[](const TransitionMember& member) { return true; }, true);

				//Dyning
				m_stateMachine->AddEdge(State::Dyning, State::Death,
					[](const TransitionMember& member) { return true; }, true);
			}

			void Stator_EvadeSlime::SettingParametor() {
				SettingPlowlingParametor();
			}

			void Stator_EvadeSlime::SettingPlowlingParametor() {
				const auto& plowlingParamPtr = m_paramPtr->plowlingParamPtr;
				plowlingParamPtr->positions = {
					Vec3(+5.0f, +0.5f, -7.0f),
					Vec3(-5.0f, +0.5f, -7.0f),
				};
				plowlingParamPtr->waitParamPtr->time = 1.0f;

				//�ړ��p�����[�^
				auto& moveParamPtr = plowlingParamPtr->moveParamPtr->moveParamPtr;
				moveParamPtr->speed = 5.0f;
				moveParamPtr->targetNearRange = 2.0f;
				moveParamPtr->moveType = Task_ToTargetMove::MoveType::ArriveVelocity;
			}

		}
	}
}