
/*!
@file Task_Preliminary_BigSlime.cpp
@brief Task_Preliminary_BigSlimeのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyBase.h"
#include "TargetManager.h"

#include "Task_Preliminary_BigSlime.h"

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

#include "EnemyObjectBase.h"
#include "SlimeObjectBase.h"
#include "AbsorbedSlimeManager.h"

#include "DebugObject.h"

namespace basecross {
	namespace Enemy {

		namespace BigSlime {

			namespace Task {

				//--------------------------------------------------------------------------------------
				/// パラメータ
				//--------------------------------------------------------------------------------------

				Preliminary_Attack_Parametor::Preliminary_Attack_Parametor()
					: Preliminary_Attack_Parametor(0.5f)
				{}

				Preliminary_Attack_Parametor::Preliminary_Attack_Parametor(const float& time)
					: time(time)
				{}

				//--------------------------------------------------------------------------------------
				/// 巨大スライムの攻撃予備動作タスク本体
				//--------------------------------------------------------------------------------------

				Preliminary_Attack::Preliminary_Attack(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr):
					TaskNodeBase<EnemyBase>(owner), m_paramPtr(paramPtr)
				{}

				void Preliminary_Attack::OnStart() {
					auto ownerObject = GetOwner()->GetGameObject();

					//アニメーションの変更
					auto absorbedManager = ownerObject->GetComponent<BigSlime::AbsorbedSlimeManagerBase>(false);
					if(absorbedManager){
						absorbedManager->ForceChangeAnimations<NormalSlime::Animator>(NormalSlime::Animator::StateType::BodyBlowPreliminary);
					}

					//速度減速開始
					auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);
					if (velocityManager) {
						velocityManager->StartDeseleration();
					}
				}

				bool Preliminary_Attack::OnUpdate() {
					auto absorbedManager = GetOwner()->GetGameObject()->GetComponent<BigSlime::AbsorbedSlimeManagerBase>(false);
					if (absorbedManager) {
						for (auto& animator : absorbedManager->GetAbsorbedComponents<NormalSlime::Animator>()) {
							auto currentNode = animator->GetCurrentNode();
							if (currentNode) {
								//Animationが終了していなかったら遷移しない
								if (!currentNode->GetAnimationClip()->IsEnd()) {
									return false;
								}
							}
						}
					}

					DebugObject::AddString(L"Task_Preliminary_NormalSlimeでAnimatorがありません。");

					return true;
				}

				void Preliminary_Attack::OnExit() {
					auto velocityManager = GetOwner()->GetGameObject()->GetComponent<VelocityManager>(false);
					if (velocityManager) {
						velocityManager->SetIsDeseleration(false);
						velocityManager->ResetAll();
					}
				}


				void Preliminary_Attack::Rotation() {
					auto ownerObject = GetOwner()->GetGameObject();
					auto rotationController = ownerObject->GetComponent<RotationController>(false);
					auto targetManager = ownerObject->GetComponent<TargetManager>(false);
					if (rotationController && targetManager && targetManager->HasTarget()) {
						rotationController->SetDirect(targetManager->CalcuToTargetVec());
					}
				}

			}
			
		}
	}
}