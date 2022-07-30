
/*!
@file Task_Preliminary_NormalSlime.cpp
@brief Task_Preliminary_NormalSlimeのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyBase.h"
#include "TargetManager.h"

#include "Task_Preliminary_NormalSlime.h"

#include "VelocityManager.h"
#include "RotationController.h"
#include "TargetManager.h"

#include "GameTimer.h"
#include "TaskList.h"
#include "SeekTargetOffsetsManager.h"
#include "SeekTarget.h"
#include "MetaballChildrenSeekManager.h"

#include "UtilityVelocity.h"

#include "MaruAnimationHelper.h"
#include "StateEdgeBase.h"
#include "StateNodeBase.h"
#include "AnimationMainStateMachine.h"
#include "AnimatorBase.h"
#include "Animator_NormalSlime.h"

#include "DebugObject.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// パラメータ
		//--------------------------------------------------------------------------------------

		Task_Preliminary_NormalSlime::Parametor::Parametor()
			:Parametor(0.5f, 1.5f)
		{}

		Task_Preliminary_NormalSlime::Parametor::Parametor(const float& time, const float& range)
			:time(time), range(range)
		{}

		//--------------------------------------------------------------------------------------
		/// 予備動作タスク本体
		//--------------------------------------------------------------------------------------

		Task_Preliminary_NormalSlime::Task_Preliminary_NormalSlime(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr)
			:TaskNodeBase<EnemyBase>(owner), m_paramPtr(paramPtr)
		{}

		void Task_Preliminary_NormalSlime::OnStart() {
			auto ownerObject = GetOwner()->GetGameObject();
			auto animator = ownerObject->GetComponent<NormalSlime::Animator>(false);
			if (animator) {
				animator->ForceChangeState(NormalSlime::Animator::StateType::BodyBlowPreliminary);
			}

			auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);
			if (velocityManager) {
				velocityManager->StartDeseleration();
			}
		}

		bool Task_Preliminary_NormalSlime::OnUpdate() {
			auto animator = GetOwner()->GetGameObject()->GetComponent<NormalSlime::Animator>(false);
			if (animator) {
				auto currentNode = animator->GetCurrentNode();
				if (currentNode) {
					return currentNode->GetAnimationClip()->IsEnd();
				}
			}

			DebugObject::AddString(L"Task_Preliminary_NormalSlimeでAnimatorがありません。");

			return true;
		}

		void Task_Preliminary_NormalSlime::OnExit() {
			auto velocityManager = GetOwner()->GetGameObject()->GetComponent<VelocityManager>(false);
			if (velocityManager) {
				velocityManager->SetIsDeseleration(false);
				velocityManager->ResetAll();
			}
		}

		void Task_Preliminary_NormalSlime::Rotation() {
			auto ownerObject = GetOwner()->GetGameObject();
			auto rotationController = ownerObject->GetComponent<RotationController>(false);
			auto targetManager = ownerObject->GetComponent<TargetManager>(false);
			if (rotationController && targetManager && targetManager->HasTarget()) {
				rotationController->SetDirect(targetManager->CalcuToTargetVec());
			}
		}

	}
}