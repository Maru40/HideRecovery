
/*!
@file Animator_NormalSlime.cpp
@brief Animator_NormalSlimeのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "AnimatorBase.h"
#include "Animator_NormalSlime.h"

#include "StateEdgeBase.h"
#include "GraphBase.h"
#include "MaruAnimationHelper.h"
#include "AnimationMainStateMachine.h"

#include "I_KnockBack.h"

#include "EnemyBase.h"

#include "GameTimer.h"
#include "StateNodeBase.h"
#include "AnimationNodeBase.h"
#include "Motion_BodyBlow.h"

#include "TaskList.h"
#include "SeekTarget.h"
#include "MetaballChildrenRender.h"
#include "MetaballMotionManager.h"

#include "Motion_GenericPreliminary.h"
#include "Motion_Idle.h"
#include "Motion_KnockBack.h"

#include "SubAnimatorBase.h"
#include "SubAnimator_Walk.h"

#include "VelocityManager.h"

#include "Utility_EnemyStator.h"

namespace basecross {
	namespace Enemy {

		namespace NormalSlime {

			//--------------------------------------------------------------------------------------
			/// ブラックボード
			//--------------------------------------------------------------------------------------

			Animator_BlackBoard::Animator_BlackBoard() :
				knockBackParamPtr(std::make_shared<Enemy::Motion::KnockBack::Parametor>()),
				pressedParamPtr(std::make_shared<Enemy::Motion::KnockBack::Parametor>(1.0f))
			{}

			//--------------------------------------------------------------------------------------
			/// 通常スライムのアニメーター本体
			//--------------------------------------------------------------------------------------

			Animator::Animator(const std::shared_ptr<GameObject>& objPtr)
				:AnimatorBase(objPtr),m_blackBoard(BlackBoard())
			{}

			void Animator::CreateNode() {
				auto enemy = GetGameObject()->GetComponent<EnemyBase>();

				//Idle
				m_stateMachine->AddNode(StateType::Idle, std::make_shared<maru::AnimationStateNodeBase>(
					std::make_shared<maru::AnimationClip>(std::make_shared<Enemy::Motion::Idle>(enemy))));

				//攻撃後硬直
				m_stateMachine->AddNode(StateType::IdleWait, std::make_shared<maru::AnimationStateNodeBase>(
					std::make_shared<maru::AnimationClip>(std::make_shared<Enemy::Motion::Idle>(enemy))));

				//移動
				m_stateMachine->AddNode(StateType::Walk, std::make_shared<SubAnimator::Walk>(GetGameObject()));

				//BodyBlow予備動作
				m_stateMachine->AddNode(StateType::BodyBlowPreliminary, std::make_shared<maru::AnimationStateNodeBase>(
					std::make_shared<maru::AnimationClip>(std::make_shared<Enemy::Motion::GenericPreliminary>(enemy))));

				//突進
				m_stateMachine->AddNode(StateType::BodyBlow, std::make_shared<maru::AnimationStateNodeBase>(
					std::make_shared<maru::AnimationClip>(std::make_shared<Motion::BodyBlow>(enemy))));

				//ノックバック
				m_stateMachine->AddNode(StateType::KnockBack, std::make_shared<maru::AnimationStateNodeBase>(
					std::make_shared<maru::AnimationClip>(std::make_shared<Enemy::Motion::KnockBack>(enemy, m_blackBoard.knockBackParamPtr))));

				//Pressed中
				m_stateMachine->AddNode(StateType::Pressed, std::make_shared<maru::AnimationStateNodeBase>(
					std::make_shared<maru::AnimationClip>(std::make_shared<Enemy::Motion::KnockBack>(enemy, m_blackBoard.pressedParamPtr))));
			}

			void Animator::CreateEdge() {
				using namespace UtilityStator;

				//Idle状態
				m_stateMachine->AddEdge(StateType::Idle, StateType::Walk,
					[&, this](const TransitionMember& member) 
					{ return Transition::ComparisonVelocity(GetGameObject(), member.toWalkSpeed, ComparisonType::Greater); });

				//Walk状態
				m_stateMachine->AddEdge(StateType::Walk, StateType::Idle,
					[&, this](const TransitionMember& member)
					{ return Transition::ComparisonVelocity(GetGameObject(), member.toWalkSpeed, ComparisonType::OrLess); });

				//ボディブロー
				//自分自身が終了したら
				m_stateMachine->AddEdge(StateType::BodyBlow, StateType::IdleWait, 
					[&, this](const TransitionMember& member) { return Transition::IsSelfSetateEnd(m_stateMachine, StateType::BodyBlow); });

				//ノックバック
				m_stateMachine->AddEdge(StateType::KnockBack, StateType::Idle,
					[&, this](const TransitionMember& member) { return true; }, true);
			}

			void Animator::SetBlackBoard(const BlackBoard& blackBoard) noexcept {
				m_blackBoard = blackBoard;
			}

			Animator::BlackBoard Animator::GetBlackBoard() const noexcept {
				return m_blackBoard;
			}

			Animator::BlackBoard& Animator::GetRefBlackBoard() {
				return m_blackBoard;
			}
		}
	}
}