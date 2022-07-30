/*!
@file ShootSlime_Attack.cpp
@brief ShootSlime_Attack�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "TaskList.h"

#include "Task_Shoot.h"
//��L�K�v�C���N���[�h-----------------------------------------------------------------

#include "EnemyBase.h"
#include "VelocityManager.h"
#include "ShootManager.h"
#include "TargetManager.h"

namespace basecross {
	namespace Enemy {
		namespace ShootSlime {

			namespace Task {

				//--------------------------------------------------------------------------------------
				/// �p�����[�^
				//--------------------------------------------------------------------------------------

				Shoot_Parametor::Shoot_Parametor()
					:Shoot_Parametor(15.0f, 0.0f)
				{}

				Shoot_Parametor::Shoot_Parametor(const float& speed, const float& backPower)
					:speed(speed), backPower(backPower)
				{}

				//--------------------------------------------------------------------------------------
				/// �V���b�g�X�e�[�g�{��
				//--------------------------------------------------------------------------------------

				Shoot::Shoot(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr)
					:TaskNodeBase(owner), m_paramPtr(paramPtr)
				{}

				void Shoot::OnStart() {
					auto ownerObject = GetOwner()->GetGameObject();
					auto targetManager = ownerObject->GetComponent<TargetManager>(false);
					auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);
					//�m�b�N�o�b�N����
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

					//����������������B
					if (velocityManager) {
						return !velocityManager->IsDeseleration();
					}
					return false;
				}
			}
		}
	}
}