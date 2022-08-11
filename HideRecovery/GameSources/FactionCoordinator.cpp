
/*!
@file FactionCoordinator.cpp
@brief FactionCoordinatorのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "FactionCoordinator.h"

#include "I_FactionMember.h"
#include "SingletonComponent.h"
#include "EnemyAIDirector.h"

#include "EnemyBase.h"

#include "PatrolCoordinator.h"
#include "CombatCoordinator.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	namespace Enemy {

		FactionCoordinator::FactionCoordinator(const std::shared_ptr<AIDirector>& director):
			FactionCoordinator(director, std::vector<std::weak_ptr<EnemyBase>>())
		{}

		FactionCoordinator::FactionCoordinator(const std::shared_ptr<AIDirector>& director, const std::vector<std::weak_ptr<EnemyBase>>& members):
			CoordinatorBase(members), m_director(director)
		{}

		void FactionCoordinator::OnStart() {
			//仮でパトロールを作成
			auto patrol = CreateFaction<PatrolCoordinator>(m_patrols, GetThis<FactionCoordinator>(), GetMembers());
		}

		void FactionCoordinator::OnUpdate() {
			for (auto& coordinator : m_allCoordinators) {
				coordinator->OnUpdate();
			}
		}

		void FactionCoordinator::OnExit() {

		}

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

	}
}