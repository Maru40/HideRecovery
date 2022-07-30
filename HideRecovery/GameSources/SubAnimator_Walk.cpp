
/*!
@file SubAnimator_Walk.cpp
@brief SubAnimator_WalkÇÃÉNÉâÉXé¿ëÃ
íSìñÅFä€éRóTäÏ
*/

#include "stdafx.h"
#include "Project.h"

#include "MaruAnimationHelper.h"
#include "SubAnimatorBase.h"
#include "SubAnimator_Walk.h"

#include "EnemyBase.h"

#include "StateEdgeBase.h"
#include "GraphBase.h"
#include "MaruAnimationHelper.h"
#include "AnimationMainStateMachine.h"

#include "TaskList.h"
#include "GameTimer.h"
#include "SeekTarget.h"
#include "MetaballChildrenRender.h"
#include "MetaballMotionManager.h"
#include "Motion_Idle.h"

#include "Motion_GenericPreliminary.h"
#include "Motion_WalkJump.h"
#include "Motion_WalkSmooth.h"

#include "Utility_EnemyStator.h"

namespace basecross {
	namespace Enemy {

		namespace SubAnimator
		{
			Walk::Walk(const std::shared_ptr<GameObject>& objPtr)
				:SubAnimatorBase(objPtr)
			{
				CreateNode();
				CreateEdge();
			}

			void Walk::CreateNode() {
				auto enemy = GetGameObject()->GetComponent<EnemyBase>();

				//Idle
				m_stateMachine->AddNode(StateType::Idle, std::make_shared<maru::AnimationStateNodeBase>(
					std::make_shared<maru::AnimationClip>(std::make_shared<Enemy::Motion::Idle>(enemy))));

				//ó\îıìÆçÏ
				m_stateMachine->AddNode(StateType::Preliminary, std::make_shared<maru::AnimationStateNodeBase>(
					std::make_shared<maru::AnimationClip>(std::make_shared<Enemy::Motion::GenericPreliminary>(enemy))));

				//Jump
				m_stateMachine->AddNode(StateType::Jump, std::make_shared<maru::AnimationStateNodeBase>(
					std::make_shared<maru::AnimationClip>(std::make_shared<Enemy::Motion::WalkJump>(enemy))));

				//Smooth
				m_stateMachine->AddNode(StateType::Smooth, std::make_shared<maru::AnimationStateNodeBase>(
					std::make_shared<maru::AnimationClip>(std::make_shared<Enemy::Motion::WalkSmooth>(enemy))));
			}

			void Walk::CreateEdge() {
				using namespace UtilityStator;

				//Idle
				m_stateMachine->AddEdge(StateType::Idle, StateType::Preliminary,
					[&, this](const TransitionMember& member) { return Transition::IsSelfSetateEnd(m_stateMachine, StateType::Idle); });

				//Preliminary
				m_stateMachine->AddEdge(StateType::Preliminary, StateType::Smooth,
					[&, this](const TransitionMember& member) { return Transition::IsSelfSetateEnd(m_stateMachine, StateType::Preliminary); });

				//Jump
				m_stateMachine->AddEdge(StateType::Jump, StateType::Idle,
					[&, this](const TransitionMember& member) { return Transition::IsSelfSetateEnd(m_stateMachine, StateType::Jump); });

				//Smoooth
				m_stateMachine->AddEdge(StateType::Smooth, StateType::Idle,
					[&, this](const TransitionMember& member) { return Transition::IsSelfSetateEnd(m_stateMachine, StateType::Smooth); });
			}

		}

	}
}