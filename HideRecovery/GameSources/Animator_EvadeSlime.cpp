
/*!
@file Animator_EvadeSlime.cpp
@brief Animator_EvadeSlime�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "AnimatorBase.h"
#include "Animator_EvadeSlime.h"

#include "StateEdgeBase.h"
#include "GraphBase.h"
#include "MaruAnimationHelper.h"
#include "AnimationMainStateMachine.h"

#include "EnemyBase.h"

#include "GameTimer.h"
#include "StateNodeBase.h"
#include "AnimationNodeBase.h"

#include "TaskList.h"
#include "SeekTarget.h"
#include "MetaballChildrenRender.h"
#include "MetaballMotionManager.h"

#include "I_KnockBack.h"
#include "Motion_KnockBack.h"
#include "Motion_GenericPreliminary.h"
#include "Motion_Idle.h"

#include "SubAnimatorBase.h"
#include "SubAnimator_WalkJump.h"

#include "VelocityManager.h"

#include "Utility_EnemyStator.h"

namespace basecross {
	namespace Enemy {

		namespace EvadeSlime {

			//--------------------------------------------------------------------------------------
			/// �u���b�N�{�[�h
			//--------------------------------------------------------------------------------------

			Animator_BlackBoard::Animator_BlackBoard() :
				knockBackParamPtr(std::make_shared<Enemy::Motion::KnockBack::Parametor>()),
				pressedParamPtr(std::make_shared<Enemy::Motion::KnockBack::Parametor>(1.0f))
			{}

			//--------------------------------------------------------------------------------------
			/// ������X���C���̃A�j���[�^�{��
			//--------------------------------------------------------------------------------------

			Animator::Animator(const std::shared_ptr<GameObject>& objPtr)
				:AnimatorBase(objPtr), m_blackBoard(BlackBoard())
			{}

			void Animator::CreateNode() {
				auto enemy = GetGameObject()->GetComponent<EnemyBase>();

				//Idle
				m_stateMachine->AddNode(StateType::Idle, std::make_shared<maru::AnimationStateNodeBase>(
					std::make_shared<maru::AnimationClip>(std::make_shared<Enemy::Motion::Idle>(enemy))));

				//�d��
				m_stateMachine->AddNode(StateType::IdleWait, std::make_shared<maru::AnimationStateNodeBase>(
					std::make_shared<maru::AnimationClip>(std::make_shared<Enemy::Motion::Idle>(enemy))));

				//�ړ�
				m_stateMachine->AddNode(StateType::Walk, std::make_shared<SubAnimator::WalkJump>(GetGameObject()));

				//�m�b�N�o�b�N
				m_stateMachine->AddNode(StateType::KnockBack, std::make_shared<maru::AnimationStateNodeBase>(
					std::make_shared<maru::AnimationClip>(std::make_shared<Enemy::Motion::KnockBack>(enemy, m_blackBoard.knockBackParamPtr))));

				//Pressed��
				m_stateMachine->AddNode(StateType::Pressed, std::make_shared<maru::AnimationStateNodeBase>(
					std::make_shared<maru::AnimationClip>(std::make_shared<Enemy::Motion::KnockBack>(enemy, m_blackBoard.pressedParamPtr))));
			}

			void Animator::CreateEdge() {
				using namespace UtilityStator;

				//Idle���
				m_stateMachine->AddEdge(StateType::Idle, StateType::Walk,
					[&, this](const TransitionMember& member)
					{ return Transition::ComparisonVelocity(GetGameObject(), member.toWalkSpeed, ComparisonType::Greater); });

				//Walk���
				m_stateMachine->AddEdge(StateType::Walk, StateType::Idle,
					[&, this](const TransitionMember& member)
					{ return Transition::ComparisonVelocity(GetGameObject(), member.toWalkSpeed, ComparisonType::OrLess); });

				//�m�b�N�o�b�N
				m_stateMachine->AddEdge(StateType::KnockBack, StateType::Idle,
					[&, this](const TransitionMember& member) { return true; }, true);
			}

			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			void Animator::SetBlackBoard(const BlackBoard& blackBoard) {
				m_blackBoard = blackBoard;
			}
			Animator::BlackBoard Animator::GetBlackBoard() const {
				return m_blackBoard;
			}
			Animator::BlackBoard& Animator::GetRefBlackBoard() {
				return m_blackBoard;
			}
		}
	}
}