/*!
@file StateNode_HidePlacePatrol.cpp
@brief StateNode_HidePlacePatrol�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "StateNode_Buttle.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Enemy/Behavior/BehaviorTree.h"
#include "Maruyama/Enemy/Behavior/SubBehaviorTree/ButtleTree.h"

#include "Maruyama/Enemy/AIDirector/CoordinatorBase.h"
#include "Maruyama/Enemy/AIDirector/TupleSpace.h"

#include "Maruyama/Utility/Component/TargetManager.h"
#include "Watanabe/Component/PlayerStatus.h"

#include "Maruyama/Enemy/AIDirector/CombatCoordinator.h"
#include "Maruyama/Enemy/AIDirector/FactionCoordinator.h"

#include "Maruyama/Interface/I_FactionMember.h"

#include "Maruyama/Enemy/AIDirector/Calculater/CombatCalculater.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			Buttle::Buttle(const std::shared_ptr<EnemyBase>& owner):
				NodeBase_StateMachine(owner),
				m_behaviorTree(new ButtleTree(owner->GetGameObject()))
			{
				auto object = owner->GetGameObject();

				m_transform = object->GetComponent<Transform>();
				m_targetManager = object->GetComponent<TargetManager>(false);

				m_behaviorTree->OnCreate();
			}

			void Buttle::OnStart() {
				//�o�g���R�[�f�B�l�[�^�[�Ɉړ�����B
				auto faction = GetOwner()->GetFactionCoordinator();
				faction->TransitionFaction<CombatCoordinator>(GetOwner());

				//�ʒm�ݒ�
				SettingNotify();
			}

			bool Buttle::OnUpdate() {
				m_behaviorTree->OnUpdate();

				return true;
			}

			void Buttle::OnExit() {
				m_behaviorTree->ForceStop();

				//�o�^�����ʒm���폜
				auto  member = GetOwner();
				auto assignedFaction = member->GetAssignedFaction();	
				assignedFaction->RemoveMember(member);
				assignedFaction->GetTupleSpace()->RemoveAllNotifys(member); //�^�v���X�y�[�X�ɓo�^���ꂽ�҂�ύX
				assignedFaction->GetTupleSpace()->RemoveAllTuples(member);
			}

			void Buttle::SettingNotify() {
				auto tupleSpace = GetOwner()->GetAssignedFaction()->GetTupleSpace();

				//�^�[�Q�b�g����
				tupleSpace->Notify<Tuple::FindTarget>(
					GetOwner(),
					[&](const std::shared_ptr<Tuple::FindTarget>& tuple) { ObserveOtherFindTarget(tuple); }
				);

				//�_���[�W�ʒm
				tupleSpace->Notify<Tuple::Damaged>(
					GetOwner(),
					[&](const std::shared_ptr<Tuple::Damaged>& tuple) { ObserveDamaged(tuple); }
				);

				//Kill�ʒm
				tupleSpace->Notify<Tuple::Kill>(
					GetOwner(),
					[&](const std::shared_ptr<Tuple::Kill>& tuple) { NotifyTuple_Kill(tuple); }
				);
			}

			void Buttle::ObserveOtherFindTarget(const std::shared_ptr<Tuple::FindTarget>& tuple) {
				//��������Ń^�[�Q�b�g�ύX������B
				if (!HasTarget()) {
					return;
				}
				
				auto targetManager = m_targetManager.lock();
				//�]���l�����̃^�[�Q�b�g��菬�����Ȃ�
				if (tuple->GetValue() < targetManager->CalcuToTargetVec().length()) {
					//�t�@�N�V�����ɁA������ڕW�ɂ��������Ƃ�`����B
					targetManager->SetTarget(tuple->GetTarget());
				}
			}

			void Buttle::ObserveDamaged(const std::shared_ptr<Tuple::Damaged>& tuple) {
				//�������g���_���[�W���󂯂����ƁA�Ⴄ���肪�_���[�W���󂯂����ŏ������Ⴄ�B
				tuple->GetRequester() == GetOwner() ? SelfDamaged(tuple) : OtherDamaged(tuple);
			}

			void Buttle::SelfDamaged(const std::shared_ptr<Tuple::Damaged>& tuple) {
				auto otherTarget = tuple->GetDamageData().attacker;

				if (!HasTarget()) {
					//�^�[�Q�b�g�������ĂȂ��Ȃ�A�^�[�Q�b�g��ݒ�B
					m_targetManager.lock()->SetTarget(otherTarget);
					return;
				}

				//�����^�[�Q�b�g�Ȃ�؂�ւ��̕K�v�͂Ȃ��B
				if (m_targetManager.lock()->GetTarget() == otherTarget) {
					return;
				}

				float currentValue = Calculater::Combat::CalculateEvalutionValue(m_transform.lock(), m_targetManager.lock()->GetTarget());
				float otherValue = Calculater::Combat::CalculateEvalutionValue(m_transform.lock(), otherTarget);

				//ohterValue�̕����D��Ȃ�A�^�[�Q�b�g��؂�ւ���B
				if (otherValue < currentValue) {
					GetOwner()->GetAssignedFaction()->GetTupleSpace()->Write<Tuple::FindTarget>(
						GetOwner(),
						otherTarget,
						otherValue
					);

					m_targetManager.lock()->SetTarget(otherTarget);
				}
			}

			void Buttle::OtherDamaged(const std::shared_ptr<Tuple::Damaged>& tuple) {
				auto otherTarget = tuple->GetDamageData().attacker;

				//�^�[�Q�b�g�����݂��Ȃ��Ȃ�A
				if (!HasTarget()) {
					m_targetManager.lock()->SetTarget(otherTarget);
					return;
				}

				//�U�����̑ΏۂȂ�A���ɉ������Ȃ��B
				if (m_targetManager.lock()->GetTarget() == otherTarget) {
					return;
				}

				//�ǂ���̃^�[�Q�b�g�̗D��x�����������v�Z����B
				float currentValue = Calculater::Combat::CalculateEvalutionValue(m_transform.lock(), m_targetManager.lock()->GetTarget());
				float otherValue = Calculater::Combat::CalculateEvalutionValue(m_transform.lock(), otherTarget);

				//ohterValue�̕����D��Ȃ�A�����邱�Ƃ�`����B
				if (otherValue < currentValue) {
					auto tupleSpace = GetOwner()->GetAssignedFaction()->GetTupleSpace();
					tupleSpace->Write<Tuple::HelpAction>(GetOwner(), otherTarget, otherValue);	//HelpWrite����B
				}
			}

			void Buttle::NotifyTuple_Kill(const std::shared_ptr<Tuple::Kill>& tuple) {
				//�^�[�Q�b�g�����݂��Ȃ��Ȃ�A�����؂�ւ�,
				//���́A�^�[�Q�b�g�����S��������Ȃ�؂�ւ���B
				if (!HasTarget() ||
					m_targetManager.lock()->GetTarget() == tuple->GetKilled()	//�^�[�Q�b�g�����S��������Ȃ�
				) {
					//�V�K�^�[�Q�b�g�̌����˗�
					GetOwner()->GetAssignedFaction()->GetTupleSpace()->Write<Tuple::SearchTarget>(
						GetOwner(),
						m_targetManager.lock(),
						0.0f
					);
				}
			}

			bool Buttle::HasTarget() const {
				auto targetManager = m_targetManager.lock();
				return targetManager->HasTarget();
			}
		}
	}
}