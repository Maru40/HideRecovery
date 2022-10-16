
/*!
@file PatrolCoordinator.cpp
@brief PatrolCoordinator�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "PatrolCoordinator.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "FactionCoordinator.h"

#include "Maruyama/Utility/Utility.h"

#include "Watanabe/DebugClass/Debug.h"

#include "Maruyama/Interface/I_FactionMember.h"

#include "Maruyama/Enemy/StateMachine/EnemyMainStateMachine.h"
#include "Maruyama/Enemy/Component/Stator/StatorBase.h"
#include "Maruyama/Enemy/Component/Stator/EnemyStatorBase.h"

#include "Maruyama/Utility/Component/TargetManager.h"

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
			PatrolControl();

			ObserveTuple_FindTarget();
		}

		void PatrolCoordinator::PatrolControl() {
			
		}

		void PatrolCoordinator::ObserveTuple_FindTarget() {
			auto tupleSpace = GetTupleSpace();
			auto tuple = tupleSpace->Take<Tuple::FindTarget>();
			if (!tuple) {
				return;
			}

			auto target = tuple->target.lock();

			for (auto& weakMember : GetMembers()) {
				auto member = weakMember.lock();

				auto stator = member->GetGameObject()->GetComponent<I_Stator_EnumType<Enemy::StateType>>(false);
				auto targetManager = member->GetGameObject()->GetComponent<TargetManager>(false);
				if (!stator || !targetManager) {
					continue;
				}

				if (stator->IsCurrentState(Enemy::StateType::Chase)) {	//�X�e�[�g�������Ȃ珈�����Ȃ�
					continue;
				}

				targetManager->SetTarget(target);
				stator->ChangeState(Enemy::StateType::Chase, (int)Enemy::StateType::Chase);
			}
		}
	}
}