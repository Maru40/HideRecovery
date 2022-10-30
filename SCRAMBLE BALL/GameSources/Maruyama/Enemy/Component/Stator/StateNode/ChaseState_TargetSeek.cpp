
/*!
@file ChaseState_TargetSeek.cpp
@brief ChaseState_TargetSeek�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Enemy/StateMachine/EnemyStateNodeBase.h"
#include "ChaseState_TargetSeek.h"

#include "Maruyama/Utility/Component/SeekTarget.h"
#include "Maruyama/Utility/Component/TargetManager.h"
#include "Maruyama/Utility/Component/RotationController.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			namespace ChaseState {

				//--------------------------------------------------------------------------------------
				/// �p�����[�^
				//--------------------------------------------------------------------------------------

				TargetSeekParametor::TargetSeekParametor()
					: TargetSeekParametor(std::make_shared<SeekTarget::Parametor>(SeekTarget::SeekType::VelcoityArrive))
				{}

				TargetSeekParametor::TargetSeekParametor(const std::shared_ptr<SeekTarget::Parametor>& seekParamPtr)
					: seekParamPtr(seekParamPtr)
				{}

				//--------------------------------------------------------------------------------------
				/// �^�[�Q�b�g�Ǐ]�X�e�[�g
				//--------------------------------------------------------------------------------------

				ChaseState_TargetSeek::ChaseState_TargetSeek(
					const std::shared_ptr<EnemyBase>& owner, 
					const std::shared_ptr<TargetSeekParametor>& parametor
				):
					EnemyStateNodeBase<EnemyBase>(owner), 
					m_paramPtr(parametor)
				{}

				void ChaseState_TargetSeek::OnStart() {
					auto seek = GetOwner()->GetGameObject()->GetComponent<SeekTarget>(false);
					auto targetManager = GetOwner()->GetGameObject()->GetComponent<TargetManager>(false);
					//�w��̃R���|�[�l���g�����������珈�����΂��B
					if (!seek || !targetManager || !targetManager->HasTarget()) {
						return;
					}

					//�^�[�Q�b�g��ݒ�
					if (seek) {
						seek->SetTarget(targetManager->GetTarget());
						seek->SetParametor(*m_paramPtr->seekParamPtr.get());
					}
				}

				bool ChaseState_TargetSeek::OnUpdate() {
					RotationUpdate();
					return false;
				}

				void ChaseState_TargetSeek::OnExit() {
					if (auto seek = GetOwner()->GetGameObject()->GetComponent<SeekTarget>(false)) {
						seek->SetTarget(nullptr);
					}
				}

				void ChaseState_TargetSeek::RotationUpdate() {
					auto targetManager = GetOwner()->GetGameObject()->GetComponent<TargetManager>(false);
					if (targetManager && !targetManager->HasTarget()) {
						return;
					}

					auto rotation = GetOwner()->GetGameObject()->GetComponent<RotationController>(false);
					if (rotation) {
						rotation->SetDirection(targetManager->CalcuToTargetVec());
					}
				}
			}
		}
	}
}