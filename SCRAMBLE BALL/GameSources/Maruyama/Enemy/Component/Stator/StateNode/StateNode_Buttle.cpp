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

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			Buttle::Buttle(const std::shared_ptr<EnemyBase>& owner):
				EnemyStateNodeBase(owner),
				m_behaviorTree(new ButtleTree(owner->GetGameObject()))
			{
				auto object = owner->GetGameObject();

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

			bool Buttle::HasTarget() const {
				auto targetManager = m_targetManager.lock();
				return targetManager->HasTarget();
			}
		}
	}
}