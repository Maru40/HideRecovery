/*!
@file IsInEyeTargets.cpp
@brief IsInEyeTargets�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "CanCurrentTarget.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Interface/I_FactionMember.h"

#include "Maruyama/Interface/I_TeamMember.h"
#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"

#include "Maruyama/Utility/Component/Targeted.h"
#include "Maruyama/Utility/Component/TargetManager.h"

#include "Maruyama/Enemy/AIDirector/CoordinatorBase.h"
#include "Maruyama/Enemy/AIDirector/TupleSpace.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				CanCurrentTarget::CanCurrentTarget(const std::shared_ptr<GameObject>& owner):
					DecoratorBase(owner)
				{
					m_targetManager = owner->GetComponent<TargetManager>(false);
					m_factionMember = owner->GetComponent<Enemy::I_FactionMember>(false);
					m_tupler = owner->GetComponent<Enemy::Tuple::I_Tupler>(false);
				}

				void CanCurrentTarget::ReserveCanTransition() {
					//�^�[�Q�b�g�����݂��Ȃ��Ȃ�A�������I��
					auto targetManager = m_targetManager.lock();
					if (!targetManager || !targetManager->HasTarget()) {
						return;
					}

					auto targeted = targetManager->GetTarget()->GetComponent<Targeted>(false);
					if (targeted && !targeted->CanTarget()) {	//�^�[�Q�b�g�̏�ԂłȂ��Ȃ�
						RemoveFactionTarget(targeted->GetGameObject());	//�^�[�Q�b�g���t�@�N�V�����̃^�[�Q�b�g��₩��O���B
						m_targetManager.lock()->SetTarget(false);
					}
				}

				bool CanCurrentTarget::CanTransition() const {
					//�^�[�Q�b�g�����݂��Ȃ��Ȃ�A�������I��
					auto targetManager = m_targetManager.lock();
					if (!targetManager || !targetManager->HasTarget()) {
						return true;
					}

					auto targeted = targetManager->GetTarget()->GetComponent<Targeted>(false);
					if (targeted && !targeted->CanTarget()) {	//�^�[�Q�b�g�̏�ԂłȂ��Ȃ�
						return false;	//�������I��������B
					}

					return true;
				}

				bool CanCurrentTarget::CanUpdate() {
					bool canTransition = CanTransition();

					if (!canTransition && m_targetManager.lock()->HasTarget()) {
						RemoveFactionTarget(m_targetManager.lock()->GetTarget());
					}

					return canTransition;
				}

				void CanCurrentTarget::RemoveFactionTarget(const std::shared_ptr<GameObject>& target) {
					auto factionMember = m_factionMember.lock();
					if (!factionMember) {
						return;
					}

					auto assignedFaction = factionMember->GetAssignedFaction();
					if (!assignedFaction) {
						return;
					}

					//�L��������������ށB
					auto tupleSpace = assignedFaction->GetTupleSpace();
					tupleSpace->Write<Enemy::Tuple::Kill>(
						m_tupler.lock(),
						GetOwner(),
						target,
						0.0f
					);
				}
			}
		}
	}
}