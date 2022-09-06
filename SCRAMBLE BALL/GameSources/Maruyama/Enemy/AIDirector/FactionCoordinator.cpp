
/*!
@file FactionCoordinator.cpp
@brief FactionCoordinator�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "FactionCoordinator.h"

#include "Maruyama/Interface/I_FactionMember.h"
#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "EnemyAIDirector.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "PatrolCoordinator.h"
#include "CombatCoordinator.h"

#include "Maruyama/Utility/Component/TargetManager.h"
#include "Maruyama/Enemy/Component/Stator/StatorBase.h"
#include "Maruyama/Enemy/Component/Stator/EnemyStatorBase.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	namespace Enemy {

		FactionCoordinator::FactionCoordinator(const std::shared_ptr<AIDirector>& director):
			FactionCoordinator(director, std::vector<std::weak_ptr<EnemyBase>>())
		{}

		FactionCoordinator::FactionCoordinator(const std::shared_ptr<AIDirector>& director, const std::vector<std::weak_ptr<EnemyBase>>& members):
			CoordinatorBase(members), m_director(director)
		{}

		void FactionCoordinator::OnStart() {
			//���Ńp�g���[�����쐬
			auto patrolMembers = GetMembers();
			auto patrol = CreateFaction<PatrolCoordinator>(m_patrols, GetThis<FactionCoordinator>(), patrolMembers);
		}

		void FactionCoordinator::OnUpdate() {
			for (auto& coordinator : m_allCoordinators) {
				coordinator->OnUpdate();
			}
		}

		void FactionCoordinator::OnExit() {

		}

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------






		//--------------------------------------------------------------------------------------
		/// �f�o�b�O
		//--------------------------------------------------------------------------------------

		void FactionCoordinator::DebugWriteTarget(const std::shared_ptr<I_FactionMember>& member, const std::shared_ptr<GameObject>& target) {
			auto patrol = member->GetAssignedFaction<PatrolCoordinator>();

			for (auto& weakMember : patrol->GetMembers()) {
				if (member == weakMember.lock()) {
					continue;
				}

				auto object = weakMember.lock()->GetGameObject();
				auto stator = object->GetComponent<I_Stator_EnumType<Enemy::StateType>>(false);
				auto targetManager = object->GetComponent<TargetManager>(false);

				if (stator && targetManager) {
					//���Œǂ�������X�e�[�g�Ȃ�X�V���Ȃ��B
					if (stator->IsCurrentState(Enemy::StateType::Chase)) {
						return;
					}

					stator->ChangeState(Enemy::StateType::Chase, (int)Enemy::StateType::Chase);
					targetManager->SetTarget(target);
				}
			}
		}
	}
}