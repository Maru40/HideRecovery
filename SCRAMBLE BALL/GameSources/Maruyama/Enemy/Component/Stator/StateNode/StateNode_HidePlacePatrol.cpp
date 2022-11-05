/*!
@file StateNode_HidePlacePatrol.cpp
@brief StateNode_HidePlacePatrol�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "StateNode_HidePlacePatrol.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Enemy/Behavior/BehaviorTree.h"
#include "Maruyama/Enemy/Behavior/SubBehaviorTree/HidePlacePatrolTree.h"

#include "Maruyama/Utility/ObserveIsInEyeTarget.h"
#include "Maruyama/Interface/I_TeamMember.h"
#include "Maruyama/Interface/I_FactionMember.h"
#include "Maruyama/Enemy/Component/EyeSearchRange.h"

#include "Maruyama/Enemy/AIDirector/FactionCoordinator.h"
#include "Maruyama/Enemy/AIDirector/TupleSpace.h"

#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			HidePlacePatrol::HidePlacePatrol(const std::shared_ptr<EnemyBase>& owner) :
				EnemyStateNodeBase(owner),
				m_behaviorTree(new HidePlacePatrolTree(owner->GetGameObject())),
				m_observeButtleTarget(new ObserveIsInEyeTarget(owner->GetGameObject()->GetComponent<EyeSearchRange>(false)))
			{
				m_behaviorTree->OnCreate();

				auto object = owner->GetGameObject();
				m_teamMember = object->GetComponent<I_TeamMember>(false);
				m_factionMember = object->GetComponent<I_FactionMember>(false);
			}

			void HidePlacePatrol::OnStart() {
				SettingObserveButtleTargets();
			}

			bool HidePlacePatrol::OnUpdate() {
				m_behaviorTree->OnUpdate();

				ObserveButtleTarget();	//�o�g���^�[�Q�b�g���Ď�����B

				return true;
			}

			void HidePlacePatrol::OnExit() {
				m_behaviorTree->ForceStop();
			}

			void HidePlacePatrol::ObserveButtleTarget() {
				//���E���ɂ���^�[�Q�b�g��S�Ď擾
				auto targets = m_observeButtleTarget->SearchIsInEyeTargets();

				//���E���̃^�[�Q�b�g�����݂��Ȃ��Ȃ�false
				if (targets.empty()) {
					return;
				}

				//�^�[�Q�b�g�����������Ƃ�AIDirector�ɓ`����B
				for (auto& target : targets) {
					auto factionMember = m_factionMember.lock();
					auto tupleSpace = factionMember->GetAssignedFaction()->GetTupleSpace();
					tupleSpace->Write<Tuple::FindTarget>(GetOwner(), target);	//�^�[�Q�b�g��ʒm
				}
			}

			void HidePlacePatrol::SettingObserveButtleTargets() {
				auto selfTeamMember = m_teamMember.lock();
				if (!selfTeamMember) {
					return;
				}

				m_observeButtleTarget->ClearObserveTargets();	//�Ď��^�[�Q�b�g�̃N���A

				//�ʃ`�[�����o�g���Ď��Ώۂɂ���B
				auto teamMembers = ShareClassesManager::GetInstance()->GetCastShareClasses<I_TeamMember>();
				for (auto& member : teamMembers) {
					//�ʃ`�[���Ȃ�A�Ď��Ώۂɒǉ�
					if (member.lock()->GetTeam() != selfTeamMember->GetTeam()) {
						m_observeButtleTarget->AddObserveTarget(member.lock()->GetOwnerObject());
					}
				}
			}

		}
	}
}