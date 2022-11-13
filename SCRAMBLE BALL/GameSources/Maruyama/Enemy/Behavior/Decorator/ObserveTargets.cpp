/*!
@file IsInEyeTargets.cpp
@brief IsInEyeTargets�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "ObserveTargets.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Interface/I_FactionMember.h"
#include "Maruyama/Enemy/Component/EyeSearchRange.h"
#include "Maruyama/Utility/Component/TargetManager.h"

#include "Maruyama/Utility/ObserveIsInEyeTarget.h"

#include "Maruyama/Enemy/AIDirector/CoordinatorBase.h"
#include "Maruyama/Enemy/AIDirector/TupleSpace.h"

#include "Maruyama/Interface/I_TeamMember.h"
#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"

#include "Watanabe/Component/PlayerStatus.h"
#include "Maruyama/Utility/Component/Targeted.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				ObserveTargets::ObserveTargets(
					const std::shared_ptr<Enemy::EnemyBase>& owner,
					const std::vector<std::shared_ptr<GameObject>>& targets
				):
					DecoratorBase(owner),
					m_observeEyeTargets(new ObserveIsInEyeTarget(owner->GetGameObject()->GetComponent<EyeSearchRange>(), targets))
				{
					auto object = owner->GetGameObject();

					m_transform = object->GetComponent<Transform>(false);
					m_teamMember = object->GetComponent<I_TeamMember>(false);
					m_targetManager = object->GetComponent<TargetManager>(false);
				}

				bool ObserveTargets::CanUpdate() {
					//�����I�ɂ͂����Ō��������Ȃ��悤�ɂ���B
					m_observeEyeTargets->ClearObserveTargets();						//�Ď��^�[�Q�b�g�̃N���A
					m_observeEyeTargets->SetObserveTargets(SearchObserveTargets());	//�^�[�Q�b�g�̎w��

					auto targets = SearchOtherTarget(m_observeEyeTargets->SearchIsInEyeTargets());	//���E���̃^�[�Q�b�g���擾

					NotifyTargets(targets);

					return true;
				}

				void ObserveTargets::NotifyTargets(const std::vector<std::shared_ptr<GameObject>>& targets) {
					using namespace basecross::Enemy;

					auto selfPosition = m_transform.lock()->GetPosition();
					auto assignedFaction = GetOwner()->GetAssignedFaction();
					auto tupleSpace = assignedFaction->GetTupleSpace();
					//�^�[�Q�b�g�����������Ƃ�ʒm����B
					for (const auto& target : targets) {
						//player�X�e�[�^�X���f�b�h��ԂȂ�A�������Ȃ�(�����I�ɂ͕ʂɂ������B)
						auto targeted = target->GetComponent<Targeted>(false);
						if (targeted && !targeted->CanTarget()) {
							continue;
						}

						//�^�[�Q�b�g�����݂̃^�[�Q�b�g�������珈�����Ȃ�
						if (target == m_targetManager.lock()->GetTarget()) {
							continue;
						}

						auto targetPosition = target->GetComponent<Transform>()->GetPosition();
						auto toTargetVec = targetPosition - selfPosition;
						tupleSpace->Write<Tuple::FindTarget>(GetOwner(), target, toTargetVec.length());
					}
				}

				std::vector<std::shared_ptr<GameObject>> ObserveTargets::SearchOtherTarget(
					const std::vector<std::shared_ptr<GameObject>>& targets
				) const 
				{
					if (!HasTarget()) {	//�^�[�Q�b�g�����݂��Ȃ��Ȃ�A���������Ȃ��B
						return targets;
					}

					std::vector<std::shared_ptr<GameObject>> result;

					//�^�[�Q�b�g�Ǘ��Ŏw�肳�ꂽ�^�[�Q�b�g���̂����B
					for (auto& target : targets) {
						if (target != m_targetManager.lock()->GetTarget()) {
							result.push_back(target);
						}
					}

					return result;
				}

				bool ObserveTargets::HasTarget() const {
					auto targetManager = m_targetManager.lock();
					return targetManager && targetManager->HasTarget();
				}

				std::vector<std::shared_ptr<GameObject>> ObserveTargets::SearchObserveTargets() const {
					std::vector<std::shared_ptr<GameObject>> result;

					auto selfTeamMember = m_teamMember.lock();
					if (!selfTeamMember) {
						return result;
					}

					//�ʃ`�[�����o�g���Ď��Ώۂɂ���B
					auto teamMembers = ShareClassesManager::GetInstance()->GetCastShareClasses<I_TeamMember>();
					for (auto& member : teamMembers) {
						//�ʃ`�[���Ȃ�A�Ď��Ώۂɒǉ�
						if (member.lock()->GetTeam() != selfTeamMember->GetTeam()) {
							result.push_back(member.lock()->GetOwnerObject());
						}
					}

					return result;
				}

			}
		}
	}
}