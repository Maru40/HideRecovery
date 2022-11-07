
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

		CombatCoordinator::CombatCoordinator(const std::shared_ptr<FactionCoordinator>& owner)
			: HereOwnerCoordinatorBase(owner)
		{}

		bool CombatCoordinator::OnUpdate() {
			for (const auto& data : m_requestDatas) {
				CreateOrder(data);
			}

			ObserveFindTarget();

			return false;
		}

		void CombatCoordinator::CreateOrder(const Data& data) {

		}

		void CombatCoordinator::ObserveFindTarget() {
			using namespace basecross::Enemy;

			auto tupleSpace = GetTupleSpace();
			auto takes = tupleSpace->Takes<Tuple::FindTarget>();	//�^�[�Q�b�g�擾�̎҂��擾
		}

		void CombatCoordinator::Request(const std::shared_ptr<EnemyBase>& member, const Data& data) {
			m_requestDatas.push_back(data);
		}

	}
}