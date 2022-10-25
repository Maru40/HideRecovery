
/*!
@file CombatCoordinator.cpp
@brief CombatCoordinatorのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "CombatCoordinator.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "FactionCoordinator.h"

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

			return false;
		}

		void CombatCoordinator::CreateOrder(const Data& data) {

		}

		void CombatCoordinator::Request(const std::shared_ptr<EnemyBase>& member, const Data& data) {
			m_requestDatas.push_back(data);
		}

	}
}