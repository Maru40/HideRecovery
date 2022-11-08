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

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			Buttle::Buttle(const std::shared_ptr<EnemyBase>& owner):
				EnemyStateNodeBase(owner),
				m_behaviorTree(new ButtleTree(owner->GetGameObject()))
			{
				auto object = owner->GetGameObject();

				m_transform = object->GetComponent<Transform>();
				m_targetManager = object->GetComponent<TargetManager>(false);

				m_behaviorTree->OnCreate();
			}

			void Buttle::OnStart() {
				//�ʒm�̎󂯎��o�^
				auto tupleSpace = GetOwner()->GetAssignedFaction()->GetTupleSpace();
				tupleSpace->Notify<Tuple::FindTarget>(
					GetOwner(), 
					[&](const std::shared_ptr<Tuple::FindTarget>& tuple) { ObserveOtherFindTarget(tuple); }
				);

				tupleSpace->Notify<Tuple::Damaged>(
					GetOwner(),
					[&](const std::shared_ptr<Tuple::Damaged>& tuple) { ObserveDamaged(tuple); }
				);
			}

			bool Buttle::OnUpdate() {
				m_behaviorTree->OnUpdate();

				return true;
			}

			void Buttle::OnExit() {
				m_behaviorTree->ForceStop();
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

				float currentValue = CalculateEvalutionValue(m_targetManager.lock()->GetTarget());
				float otherValue = CalculateEvalutionValue(otherTarget);

				//ohterValue�̕����D��Ȃ�A�^�[�Q�b�g��؂�ւ���B
				if (otherValue < currentValue) {
					m_targetManager.lock()->SetTarget(otherTarget);
				}
			}

			void Buttle::OtherDamaged(const std::shared_ptr<Tuple::Damaged>& tuple) {
				auto otherTarget = tuple->GetDamageData().attacker;

				if (!HasTarget()) {
					m_targetManager.lock()->SetTarget(otherTarget);
					return;
				}

				//�U�����̑ΏۂȂ�A���ɉ������Ȃ��B
				if (m_targetManager.lock()->GetTarget() == otherTarget) {
					return;
				}

				//�U���ΏۈȊO�Ȃ�AHelp
				float currentValue = CalculateEvalutionValue(m_targetManager.lock()->GetTarget());
				float otherValue = CalculateEvalutionValue(otherTarget);

				//ohterValue�̕����D��Ȃ�A�^�[�Q�b�g��؂�ւ���B
				if (otherValue < currentValue) {
					auto tupleSpace = GetOwner()->GetAssignedFaction()->GetTupleSpace();
					//tupleSpace->Write<Tuple::>();	//HelpWrite����B
				}
				
			}

			float Buttle::CalculateEvalutionValue(const std::shared_ptr<GameObject>& target) {
				float result = 0.0f;

				auto targetTrans = target->GetComponent<Transform>();

				float toTargetRange = (targetTrans->GetPosition() - m_transform.lock()->GetPosition()).length();
				result += toTargetRange;	//������]���l�Ƃ���B

				if (auto status = target->GetComponent<PlayerStatus>(false)) {
					float hpRate = (float)status->GetStatus().hp / (float)status->GetStatus().maxHp;
					result *= hpRate;	//hp���Ⴂ�قǁA����������(�^�[�Q�b�g�ɂ��₷������B)
				}

				return result;
			}

			bool Buttle::HasTarget() const {
				auto targetManager = m_targetManager.lock();
				return targetManager->HasTarget();
			}
		}
	}
}