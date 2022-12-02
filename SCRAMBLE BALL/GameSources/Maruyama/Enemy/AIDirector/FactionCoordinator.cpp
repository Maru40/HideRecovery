
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
#include "Maruyama/Stator/StatorBase.h"
#include "Maruyama/Enemy/Component/Stator/EnemyStatorBase.h"

#include "PatrolCoordinator/HidePlacePatrol.h"
#include "Maruyama/Interface/I_TeamMember.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	namespace Enemy {

		FactionCoordinator::FactionCoordinator(const std::shared_ptr<AIDirector>& director):
			FactionCoordinator(director, std::vector<std::weak_ptr<EnemyBase>>())
		{}

		FactionCoordinator::FactionCoordinator(const std::shared_ptr<AIDirector>& director, const std::vector<std::weak_ptr<EnemyBase>>& members):
			CoordinatorBase(members), m_director(director)
		{}

		void FactionCoordinator::OnCreate() {
			//���߂̓p�g���[���ݒ������B
			CreateFaction<AICoordinator::Patrol::HidePlacePatrol>(GetMembers());
		}

		void FactionCoordinator::OnStart() {
			//�ʒm�ݒ�
			GetDirector()->GetTupleSpace()->Notify<Tuple::FindBall>(
				GetThis<FactionCoordinator>(),
				[&](const std::shared_ptr<Tuple::FindBall>& tuple) { NotifyTuple_FindBall(tuple); }
			);
		}

		bool FactionCoordinator::OnUpdate() {
			for (auto& coordinator : m_allCoordinators) {
				coordinator->OnUpdate();
			}

			return false;
		}

		void FactionCoordinator::OnExit() {
			//�ʒm�폜
			GetDirector()->GetTupleSpace()->RemoveAllNotifys(GetThis<FactionCoordinator>());
		}

		//�{�[�����������ʒm���󂯎��B
		void FactionCoordinator::NotifyTuple_FindBall(const std::shared_ptr<Tuple::FindBall>& tuple) {
			for (auto& weakMember : GetMembers()) {
				auto member = weakMember.lock();
				auto teamMember = member->GetSelfObject()->GetComponent<I_TeamMember>(false);
				if (!teamMember) {	//�`�[�������o�[�łȂ�������
					continue;
				}

				//�������{�l��������A�S�[����ڎw���B
				if (member == tuple->GetRequester()) {
						//�S�[����ڎw�����߂��o���B
					continue;	//���̌�̏��������Ȃ��B
				}

				//�{�[���𔭌��������肪�A�����Ɠ��������o�[�Ȃ�B
				if (teamMember->GetTeam() == tuple->GetTeamMember()->GetTeam()) {
						//�w���v�s����A�W�c�s�������B
				}
				else {	//�����������肪�A�Ⴄ�����o�[�Ȃ�B
						//�{�[���������Ă��鑊����^�[�Q�b�g�ɂ���B
				}
			}
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