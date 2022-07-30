
/*!
@file Animator_BigSlime.cpp
@brief Animator_BigSlimeのクラス実体
担当：丸山裕喜
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
			/// ブラックボード
			//--------------------------------------------------------------------------------------

			Animator_BlackBoard::Animator_BlackBoard() :
				knockBackParamPtr(std::make_shared<Enemy::Motion::KnockBack::Parametor>()),
				pressedParamPtr(std::make_shared<Enemy::Motion::KnockBack::Parametor>(1.0f))
			{}

			//--------------------------------------------------------------------------------------
			/// 巨大スライムアニメーター本体
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