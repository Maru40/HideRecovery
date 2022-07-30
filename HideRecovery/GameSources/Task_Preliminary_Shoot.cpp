/*!
@file ShootSlime_Attack.cpp
@brief ShootSlime_Attackのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "TaskList.h"

#include "ShootManager.h"
#include "Task_Preliminary_Shoot.h"

#include "EnemyBase.h"
#include "VelocityManager.h"
#include "TargetManager.h"
#include "RotationController.h"

namespace basecross {
	namespace Enemy {
		namespace ShootSlime {

			namespace Task {

				//--------------------------------------------------------------------------------------
				/// パラメータ
				//--------------------------------------------------------------------------------------

				Preliminary_Shoot_Parametor::Preliminary_Shoot_Parametor()
					: Preliminary_Shoot_Parametor(std::make_shared<ShootChargeData>())
				{}

				Preliminary_Shoot_Parametor::Preliminary_Shoot_Parametor(const std::shared_ptr<ShootChargeData>& chargeDataPtr)
					: chargeDataPtr(chargeDataPtr)
				{}

				//--------------------------------------------------------------------------------------
				/// ショット攻撃の予備動作タスク本体
				//--------------------------------------------------------------------------------------

				Preliminary_Shoot::Preliminary_Shoot(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr)
					:TaskNodeBase(owner), m_paramPtr(paramPtr)
				{}

				void Preliminary_Shoot::OnStart() {
					auto ownerObject = GetOwner()->GetGameObject();
					//頭上に集める行動開始
					auto shootManager = ownerObject->GetComponent<ShootManager>(false);
					if (shootManager) {
						shootManager->StartCharge(*m_paramPtr->chargeDataPtr.get());
					}

					//減速処理
					auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);
					if (velocityManager) {
						velocityManager->StartDeseleration();
					}
				}

				bool Preliminary_Shoot::OnUpdate() {
					Rotation();
					return IsEnd();
				}

				void Preliminary_Shoot::OnExit() {
					auto ownerObject = GetOwner()->GetGameObject();
					if (!IsEnd()) { //Shootが終了していなかったら
						//ShootをBreakする処理
						auto shootManager = ownerObject->GetComponent<ShootManager>(false);
						if (shootManager) {
							shootManager->Break();
						}
					}

					//減速処理停止
					auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);
					if (velocityManager) {
						velocityManager->SetIsDeseleration(false);
					}
				}

				void Preliminary_Shoot::Rotation() {
					auto ownerObject = GetOwner()->GetGameObject();
					auto rotationController = ownerObject->GetComponent<RotationController>(false);
					auto targetManager = ownerObject->GetComponent<TargetManager>(false);
					if (rotationController && targetManager && targetManager->HasTarget()) {
						rotationController->SetDirect(targetManager->CalcuToTargetVec());
					}
				}

				bool Preliminary_Shoot::IsEnd() {
					auto ownerObject = GetOwner()->GetGameObject();
					auto shootManager = ownerObject->GetComponent<ShootManager>(false);
					if (shootManager) {
						return shootManager->IsEndCharge();
					}

					return true;
				}

			}
		}
	}
}