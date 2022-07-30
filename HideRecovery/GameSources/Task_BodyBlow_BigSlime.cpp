
/*!
@file Task_BodyBlow_BigSlime.cpp
@brief Task_BodyBlow_BigSlimeのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "TaskList.h"
#include "Task_BodyBlow.h"
#include "Task_BodyBlow_BigSlime.h"

#include "EnemyBase.h"
#include "I_BrackBoard.h"
#include "StateNodeBase.h"
#include "EnemyStateNodeBase.h"
#include "Task_Wait.h"
#include "Slime_Attack.h"
#include "I_Damaged.h"

#include "EnemyObjectBase.h"
#include "SlimeObjectBase.h"
#include "AbsorbedSlimeManager.h"

#include "PressData.h"
#include "GameTimer.h"
#include "EnemyMainStateMachine.h"
#include "StatorBase.h"
#include "SlimeStatorBase.h"
#include "Stator_NormalSlime.h"

#include "AnimationMainStateMachine.h"
#include "AnimatorBase.h"
#include "Animator_NormalSlime.h"

#include "RotationController.h"

#include "ToTargetMove.h"

#include "TargetManager.h"
#include "UtilityVelocity.h"
#include "VelocityManager.h"

#include "CollisionAction.h"

#include "EnemyStatusBase.h"

#include "DebugObject.h"

namespace basecross {
	namespace Enemy {

		namespace BigSlime {

			namespace Task {

				//--------------------------------------------------------------------------------------
				/// パラメータ
				//--------------------------------------------------------------------------------------

				BodyBlow_Parametor::BodyBlow_Parametor():
					sumAbsorbedParametor(Task_BodyBlow::Parametor(0.0f))
				{}

				//--------------------------------------------------------------------------------------
				/// 巨大スライムのボディーブロータスクの本体
				//--------------------------------------------------------------------------------------

				BodyBlow::BodyBlow(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr) :
					TaskNodeBase(owner), m_paramPtr(paramPtr)
				{
					auto collisionAction = owner->GetGameObject()->GetComponent<maru::CollisionAction>(false);
					if (collisionAction) {
						collisionAction->AddCollisionEnter([&, this](const CollisionPair& pair) { CollisionEnter(pair); });
					}
				}

				void BodyBlow::OnStart() {
					m_isActive = true;
					
					auto absorbedManager = GetOwner()->GetGameObject()->GetComponent<BigSlime::AbsorbedSlimeManagerBase>(false);
					if (absorbedManager) {
						//アニメーションの再生
						absorbedManager->ChangeAnimations<NormalSlime::Animator>(NormalSlime::Animator::StateType::BodyBlow);

						//パラメータの合計取得
						m_paramPtr->sumAbsorbedParametor = Task_BodyBlow::Parametor(0.0f);
						for (auto stator : absorbedManager->GetAbsorbedComponents<Stator_NormalSlime>()) {
							auto paramPtr = (stator->GetBlackBoardParam().attackParamPtr->bodyBlowParamPtr);
							m_paramPtr->sumAbsorbedParametor.range += paramPtr->range;
							m_paramPtr->sumAbsorbedParametor.speed += paramPtr->speed;
							m_paramPtr->sumAbsorbedParametor.toughness += paramPtr->toughness;
							*(m_paramPtr->sumAbsorbedParametor.damageData.get()) += *(paramPtr->damageData.get());
						}
						m_paramPtr->sumAbsorbedParametor.endVelocitySpeed = 0.5f;  //終了スピードは固定。

						//statusのタフネスを変更
						auto statusManager = GetOwner()->GetGameObject()->GetComponent<EnemyStatusManagerBase>(false);
						if (statusManager) {
							auto& status = statusManager->GetRefStatus();
							status.toughness = m_paramPtr->sumAbsorbedParametor.toughness;
						}
					}
				}

				bool BodyBlow::OnUpdate() {
					auto absorbedManager = GetOwner()->GetGameObject()->GetComponent<BigSlime::AbsorbedSlimeManagerBase>(false);
					if (!absorbedManager) {
						return true;
					}

					//移動Update
					if (!m_paramPtr->sumAbsorbedParametor.IsMoveEnd()) { //Moveが終了していないなら
						MoveUpdate();
						Rotation();
						ElapsedMoveRangeUpdate();
					}

					//全ての攻撃処理が終わっているため、行動を終了
					return IsEnd();
				}

				void BodyBlow::OnExit() {
					auto ownerObject = GetOwner()->GetGameObject();
					auto rotationController = ownerObject->GetComponent<RotationController>(false);
					auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);
					auto absorbedManager = ownerObject->GetComponent<AbsorbedSlimeManagerBase>(false);

					if (rotationController) {
						rotationController->SetDirect(ownerObject->GetComponent<Transform>()->GetForward());
					}

					if (absorbedManager) {
						//アニメーターの制御
						absorbedManager->ChangeAnimations<NormalSlime::Animator>(NormalSlime::Animator::StateType::Idle);
					}

					//statusのタフネスを元に戻す。
					auto statusManager = GetOwner()->GetGameObject()->GetComponent<EnemyStatusManagerBase>(false);
					if (statusManager) {
						auto& status = statusManager->GetRefStatus();
						status.toughness = status.baseToughness;
					}

					m_paramPtr->sumAbsorbedParametor.elapsedRange = m_paramPtr->sumAbsorbedParametor.range; //終了状態にする。
					m_isActive = false;
				}

				void BodyBlow::MoveUpdate() {
					auto ownerObject = GetOwner()->GetGameObject();
					auto targetManager = ownerObject->GetComponent<TargetManager>(false);
					auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);
					//ターゲットマネージャ―が存在しない、又は、ターゲットが存在しない、又は、velocityManagerが存在しない。
					if (targetManager == nullptr || !targetManager->HasTarget() || velocityManager == nullptr) {
						return;
					}

					auto direct = targetManager->CalcuToTargetVec();
					auto force = UtilityVelocity::CalucSeekVec(velocityManager->GetVelocity(), direct, m_paramPtr->sumAbsorbedParametor.speed);

					velocityManager->AddForce(force);
				}

				void BodyBlow::Rotation() {
					auto ownerObject = GetOwner()->GetGameObject();
					auto rotationController = ownerObject->GetComponent<RotationController>(false);
					auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);

					if (rotationController && velocityManager) {
						if (velocityManager->GetVelocity() == Vec3(0.0f)) {
							return;
						}

						rotationController->SetDirect(velocityManager->GetVelocity());
					}
				}

				void BodyBlow::ElapsedMoveRangeUpdate() {
					auto ownerObject = GetOwner()->GetGameObject();
					auto ownerTrans = ownerObject->GetComponent<Transform>();
					auto moveRange = (ownerTrans->GetPosition() - ownerTrans->GetBeforePosition()).length();
					m_paramPtr->sumAbsorbedParametor.elapsedRange += moveRange;

					auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);
					if (m_paramPtr->sumAbsorbedParametor.IsMoveEnd()) {
						velocityManager->StartDeseleration();
					}
				}

				void BodyBlow::CollisionEnter(const CollisionPair& pair) {
					if (m_isActive) {     //ダメージを与える状態なら。
						SendDamage(pair);
					}
				}

				void BodyBlow::SendDamage(const CollisionPair& pair) {
					auto other = pair.m_Dest.lock();

					auto damaged = other->GetGameObject()->GetComponent<I_Damaged>(false);
					if (damaged) {
						auto damageData = *m_paramPtr->sumAbsorbedParametor.damageData.get();
						damageData.hitPoint = pair.m_CalcHitPoint;
						damaged->Damaged(damageData);
					}
				}

				bool BodyBlow::IsEnd() {
					//減速が完全に完了したら。
					auto ownerObject = GetOwner()->GetGameObject();
					auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);
					if (!velocityManager) {
						DebugObject::AddString(L"Task_BodyBlow::IsEnd()、VelocityManagerが存在しません。");
						return true;
					}

					//減速して、指定の距離を走り終えたら終了
					return (velocityManager->GetVelocity().length() < m_paramPtr->sumAbsorbedParametor.endVelocitySpeed) && 
					 	   (m_paramPtr->sumAbsorbedParametor.IsMoveEnd());
				}
			}
		}
	}
}