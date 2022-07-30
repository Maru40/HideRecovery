
/*!
@file SubAnimator_WalkJump.cpp
@brief SubAnimator_WalkJumpÇÃÉNÉâÉXé¿ëÃ
íSìñÅFä€éRóTäÏ
*/

#include "stdafx.h"
#include "Project.h"

#include "MaruAnimationHelper.h"
#include "SubAnimatorBase.h"
#include "SubAnimator_WalkJump.h"

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
			WalkJump::WalkJump(const std::shared_ptr<GameObject>& objPtr)
				:SubAnimatorBase(objPtr)
			{
				CreateNode();
				CreateEdge();
			}

			void WalkJump::CreateNode() {
				auto enemy = GetGameObject()->GetComponent<EnemyBase>();

				//Idle
				auto idleParam = Enemy::Motion::Idle::Parametor(0.25f, 0.5f);
				m_stateMachine->AddNode(StateType::Idle, std::make_shared<maru::AnimationStateNodeBase>(
					std::make_shared<maru::AnimationClip>(std::make_shared<Enemy::Motion::Idle>(enemy, idleParam))));

				//ó\îıìÆçÏ
				m_stateMachine->AddNode(StateType::Preliminary, std::make_shared<maru::AnimationStateNodeBase>(
					std::make_shared<maru::AnimationClip>(std::make_shared<Enemy::Motion::GenericPreliminary>(enemy))));

				//Jump
				m_stateMachine->AddNode(StateType::Jump, std::make_shared<maru::AnimationStateNodeBase>(
					std::make_shared<maru::AnimationClip>(std::make_shared<Enemy::Motion::WalkJump>(enemy))));
			}

			void WalkJump::CreateEdge() {
				using namespace UtilityStator;

				//Idle
				m_stateMachine->AddEdge(StateType::Idle, StateType::Preliminary,
					[&, this](const TransitionMember& member) { return Transition::IsSelfSetateEnd(m_stateMachine, StateType::Idle); });

				//ó\îıìÆçÏ
				m_stateMachine->AddEdge(StateType::Preliminary, StateType::Jump,
					[&, this](const TransitionMember& member) { return Transition::IsSelfSetateEnd(m_stateMachine, StateType::Preliminary); });

				//Jump
				m_stateMachine->AddEdge(StateType::Jump, StateType::Idle,
					[&, this](const TransitionMember& member) { return Transition::IsSelfSetateEnd(m_stateMachine, StateType::Jump); });
			}

		}

	}
}