/*!
@file ShootSlime_Attack.cpp
@brief ShootSlime_Attackのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "TaskList.h"

#include "Task_Shoot.h"
//上記必要インクルード-----------------------------------------------------------------

#include "EnemyBase.h"
#include "VelocityManager.h"
#include "ShootManager.h"
#include "TargetManager.h"

namespace basecross {
	namespace Enemy {
		namespace ShootSlime {

			namespace Task {

				//--------------------------------------------------------------------------------------
				/// パラメータ
				//--------------------------------------------------------------------------------------

				Shoot_Parametor::Shoot_Parametor()
					:Shoot_Parametor(15.0f, 0.0f)
				{}

				Shoot_Parametor::Shoot_Parametor(const float& speed, const float& backPower)
					:speed(speed), backPower(backPower)
				{}

				//--------------------------------------------------------------------------------------
				/// ショットステート本体
				//--------------------------------------------------------------------------------------

				Shoot::Shoot(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr)
					:TaskNodeBase(owner), m_paramPtr(paramPtr)
				{}

				void Shoot::OnStart() {
					auto ownerObject = GetOwner()->GetGameObject();
					auto targetManager = ownerObject->GetComponent<TargetManager>(false);
					auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);
					//ノックバック処理
					if (velocityManager && targetManager && targetManager->HasTarget()) {
						auto direct = -targetManager->CalcuToTargetVec();
						velocityManager->ResetAll();
						velocityManager->SetVelocity(direct.GetNormalized() * m_paramPtr->backPower);
						velocityManager->StartDeseleration();
					}

					auto shootManager = ownerObject->GetComponent<ShootManager>(false);
					if (shootManager && targetManager && targetManager->HasTarget()) {
						shootManager->Shoot(m_paramPtr->speed, targetManager->GetTarget());
					}
					else {
						shootManager->Shoot(m_paramPtr->speed, targetManager->GetLostPosition());
					}
				}

				bool Shoot::OnUpdate() {
					return IsEnd();
				}

				void Shoot::OnExit() {
					auto ownerObject = GetOwner()->GetGameObject();
					auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);
					if (velocityManager) {
						velocityManager->SetIsDeseleration(false);
					}
				}

				bool Shoot::IsEnd() {
					auto ownerObject = GetOwner()->GetGameObject();
					auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);

					//減速が完了したら。
					if (velocityManager) {
						return !velocityManager->IsDeseleration();
					}
					return false;
				}
			}
		}
	}
}