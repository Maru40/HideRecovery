
/*!
@file CombatCoordinator.cpp
@brief CombatCoordinator�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "CombatCoordinator.h"

#include "EnemyBase.h"
#include "FactionCoordinator.h"

#include "MaruUtility.h"

namespace basecross {

	namespace Enemy {

		CombatCoordinator::CombatCoordinator(const std::shared_ptr<FactionCoordinator>& owner)
			: HereOwnerCoordinatorBase(owner)
		{}

		void CombatCoordinator::OnUpdate() {
			for (const auto& data : m_requestDatas) {
				CreateOrder(data);
			}
		}

		void CombatCoordinator::CreateOrder(const Data& data) {

		}

		void CombatCoordinator::Request(const std::shared_ptr<EnemyBase>& member, const Data& data) {
			m_requestDatas.push_back(data);
		}

	}
}