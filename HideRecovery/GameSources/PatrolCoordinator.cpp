
/*!
@file PatrolCoordinator.cpp
@brief PatrolCoordinator�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "PatrolCoordinator.h"

#include "EnemyBase.h"
#include "FactionCoordinator.h"

#include "MaruUtility.h"

#include "Watanabe/DebugClass/Debug.h"

#include "I_FactionMember.h"

namespace basecross {

	namespace Enemy {

		PatrolCoordinator::PatrolCoordinator(const std::shared_ptr<FactionCoordinator>& owner):
			PatrolCoordinator(owner, std::vector<std::weak_ptr<EnemyBase>>())
		{}

		PatrolCoordinator::PatrolCoordinator(const std::shared_ptr<FactionCoordinator>& owner, const std::vector<std::weak_ptr<EnemyBase>>& members):
			HereOwnerCoordinatorBase(owner, members)
		{}

		void PatrolCoordinator::OnStart() {
			//�����o�[�ɃO���[�v�����҂�^�C�v��`����B
			for (auto& member : GetMembers()) {
				if (auto iFactionMember = member.lock()->GetGameObject()->GetComponent<I_FactionMember>(false)) {
					//�����I�Ƀf�[�^�ł܂Ƃ߂�B
					iFactionMember->SetFactionType(FactionType::Patrol);
					iFactionMember->SetFactionCoordinator(GetOwner());
					iFactionMember->SetAssignFaction(GetThis<PatrolCoordinator>());
				}
			}

			//�����o�[�̖ړI�n�����ꂼ��ύX

		}

		void PatrolCoordinator::OnUpdate() {
			
		}
	}
}