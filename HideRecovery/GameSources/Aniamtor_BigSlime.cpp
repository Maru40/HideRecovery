
/*!
@file Animator_BigSlime.cpp
@brief Animator_BigSlime�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "AnimatorBase.h"
#include "Animator_BigSlime.h"

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
#include "Task_Wait.h"
#include "Task_BodyBlow.h"

#include "Motion_GenericPreliminary.h"
#include "Motion_Idle.h"
#include "Motion_KnockBack.h"

#include "SubAnimatorBase.h"
#include "SubAnimator_Walk.h"

#include "VelocityManager.h"

#include "Utility_EnemyStator.h"

namespace basecross {
	namespace Enemy {

		namespace BigSlime {

			//--------------------------------------------------------------------------------------
			/// �u���b�N�{�[�h
			//--------------------------------------------------------------------------------------

			Animator_BlackBoard::Animator_BlackBoard() :
				knockBackParamPtr(std::make_shared<Enemy::Motion::KnockBack::Parametor>()),
				pressedParamPtr(std::make_shared<Enemy::Motion::KnockBack::Parametor>(1.0f))
			{}

			//--------------------------------------------------------------------------------------
			/// ����X���C���A�j���[�^�[�{��
			//--------------------------------------------------------------------------------------

			Animator::Animator(const std::shared_ptr<GameObject>& objPtr)
				:AnimatorBase(objPtr), m_blackBoard(BlackBoard())
			{}

			void Animator::CreateNode() {
				auto enemy = GetGameObject()->GetComponent<EnemyBase>();
			}

			void Animator::CreateEdge() {

			}

		}
	}
}