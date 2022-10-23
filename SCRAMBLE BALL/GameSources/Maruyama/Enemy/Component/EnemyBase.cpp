/*!
@file AIPlayerBehaviorTree.cpp
@brief AIPlayerBehaviorTree‚È‚ÇÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyBase.h"

#include "Maruyama/Utility/Component/TargetManager.h"

namespace basecross {
	namespace Enemy {

		void EnemyBase::OnStart() {
			m_targetManager = GetGameObject()->GetComponent<TargetManager>(false);
		}

		void EnemyBase::SetTarget(const std::shared_ptr<GameObject>& target) {
			if (!m_targetManager.lock()) {
				return;
			}

			m_targetManager.lock()->SetTarget(target);
		}

		std::shared_ptr<GameObject> EnemyBase::GetTarget() const {
			return m_targetManager.lock() ? m_targetManager.lock()->GetTarget() : nullptr;
		}

	}
}