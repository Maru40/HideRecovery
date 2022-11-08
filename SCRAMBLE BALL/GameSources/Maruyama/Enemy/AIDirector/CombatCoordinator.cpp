
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

		bool CombatCoordinator::OnUpdate() {
			UpdateObserveFindTarget();
			UpdateObserveDamaged();

			return false;
		}

		void CombatCoordinator::UpdateObserveFindTarget() {
			auto tupleSpace = GetTupleSpace();
			auto takes = tupleSpace->Takes<Tuple::FindTarget>();	//�^�[�Q�b�g�擾�̎҂��擾
		}

		void CombatCoordinator::UpdateObserveDamaged() {
			auto takes = GetTupleSpace()->Takes<Tuple::Damaged>();

			//�����̃^�[�Q�b�g�ƈႤ�Ȃ�Help

			//Help������Ȃ灨HP���Ⴂ�����U��

			//Help�����Ȃ��Ȃ灨������B
		}

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		void CombatCoordinator::AddTargets(const std::shared_ptr<GameObject>& target) {
			//�o�^����Ă��Ȃ�������A�ǉ�����B
			if (!HasTarget(target)) {
				m_targets.push_back(target);
			}
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