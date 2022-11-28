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
#include "Maruyama/Enemy/AIDirector/PatrolCoordinator/HidePlacePatrol.h"
#include "Maruyama/Enemy/AIDirector/TupleSpace.h"

#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"
#include "Watanabe/Component/PlayerStatus.h"
#include "Maruyama/Utility/Component/TargetManager.h"

#include "Maruyama/Utility/Component/Targeted.h"

#include "Watanabe/DebugClass/Debug.h"

//#include "Maruyama/Enemy/Component/AIVirtualController.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			HidePlacePatrol::HidePlacePatrol(const std::shared_ptr<EnemyBase>& owner) :
				NodeBase_StateMachine(owner),
				m_behaviorTree(new HidePlacePatrolTree(owner->GetGameObject())),
				m_observeButtleTarget(new ObserveIsInEyeTarget(owner->GetGameObject()->GetComponent<EyeSearchRange>(false)))
			{
				m_behaviorTree->OnCreate();

				auto object = owner->GetGameObject();
				m_teamMember = object->GetComponent<I_TeamMember>(false);
				m_factionMember = object->GetComponent<I_FactionMember>(false);
				m_targetManager = object->GetComponent<TargetManager>(false);
			}

			void HidePlacePatrol::OnStart() {
				SettingObserveButtleTargets();

				//�p�g���[���t�@�N�V�����Ɉړ�
				auto factionCoordinator = GetOwner()->GetFactionCoordinator();
				factionCoordinator->TransitionFaction<AICoordinator::Patrol::HidePlacePatrol>(GetOwner());

				//�_���[�W�ʒm���󂯎���悤�ɂ���B
				if (auto factionMember = m_factionMember.lock()) {
					factionMember->GetAssignedFaction()->GetTupleSpace()->Notify<Tuple::Damaged>(
						GetOwner(),
						[&](const std::shared_ptr<Tuple::Damaged>& tuple) { Damaged(tuple); },
						[&](const std::shared_ptr<Tuple::Damaged>& tuple) { return tuple->GetRequester() == GetOwner(); }
					);
				}
			}

			bool HidePlacePatrol::OnUpdate() {
				m_behaviorTree->OnUpdate();

				ObserveButtleTarget();	//�o�g���^�[�Q�b�g���Ď�����B

				return true;
			}

			void HidePlacePatrol::OnExit() {
				m_behaviorTree->ForceStop();

				auto  member = GetOwner();
				auto assignedFaction = member->GetAssignedFaction();
				assignedFaction->RemoveMember(member);	//�t�@�N�V�������痣�E�B
				assignedFaction->GetTupleSpace()->RemoveAllNotifys(member); //�^�v���X�y�[�X�ɓo�^���ꂽ�҂�ύX
				assignedFaction->GetTupleSpace()->RemoveAllTuples(member);
			}

			void HidePlacePatrol::Damaged(const std::shared_ptr<Tuple::Damaged>& tuple) {
				//�_���[�W��^���Ă��������`����B
				auto tupleSpace = m_factionMember.lock()->GetAssignedFaction()->GetTupleSpace();
				tupleSpace->Write<Tuple::FindTarget>(GetOwner(), tuple->GetDamageData().attacker, tuple->GetValue());
				tupleSpace->Take(tuple);	//�^�v�����������g�̎҂Ƃ���B
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
					auto targeted = target->GetComponent<Targeted>(false);
					if (targeted && !targeted->CanTarget()) {
						continue;
					}

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