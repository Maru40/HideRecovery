
/*!
@file CombatCoordinator.cpp
@brief CombatCoordinator�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "CombatCoordinator.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "FactionCoordinator.h"
#include "Maruyama/Enemy/AIDirector/TupleSpace.h"

#include "Maruyama/Utility/Utility.h"

namespace basecross {

	namespace Enemy {

		using namespace basecross::Enemy;

		CombatCoordinator::CombatCoordinator(const std::shared_ptr<FactionCoordinator>& owner)
			: HereOwnerCoordinatorBase(owner)
		{
			
		}

		void CombatCoordinator::OnCreate() {
			
		}

		void CombatCoordinator::OnStart() {
			//�ʒm�ݒ�
			GetTupleSpace()->Notify<Tuple::Kill>(
				GetThis<CombatCoordinator>(),
				[&](const std::shared_ptr<Tuple::Kill>& tuple) { NotifyTuple_Kill(tuple); }
			);


		}

		bool CombatCoordinator::OnUpdate() {
			UpdateObserveFindTarget();
			UpdateObserveDamaged();

			return false;
		}

		void CombatCoordinator::OnExit() {
			GetTupleSpace()->RemoveAllNotifys(GetThis<CombatCoordinator>());
		}

		void CombatCoordinator::UpdateObserveFindTarget() {
			auto tupleSpace = GetTupleSpace();
			auto takes = tupleSpace->Takes<Tuple::FindTarget>();	//�^�[�Q�b�g�擾�̎҂��擾
		}

		void CombatCoordinator::UpdateObserveDamaged() {
			auto takes = GetTupleSpace()->Takes<Tuple::Damaged>();
		}

		void CombatCoordinator::NotifyTuple_Kill(const std::shared_ptr<Tuple::Kill>& tuple) {
			GetTupleSpace()->Take(tuple);
		}

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		void CombatCoordinator::AddTarget(const std::shared_ptr<GameObject>& target) {
			//�o�^����Ă��Ȃ�������A�ǉ�����B
			if (!HasTarget(target)) {
				m_targets.push_back(target);
			}
		}

		bool CombatCoordinator::RemoveTaret(const std::shared_ptr<GameObject>& removeTaret) {
			auto iter = m_targets.begin();
			while (iter != m_targets.end()) {
				if ((*iter).lock() == removeTaret) {
					iter = m_targets.erase(iter);
					return true;
				}
				iter++;
			}

			return false;
		}

		bool CombatCoordinator::HasTarget(const std::shared_ptr<GameObject>& target) const {
			for (const auto& weakTarget : m_targets) {
				if (weakTarget.lock() == target) {
					return true;
				}
			}
			
			return false;
		}

	}
}