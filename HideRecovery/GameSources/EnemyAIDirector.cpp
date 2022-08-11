
/*!
@file EnemyAIDirector.cpp
@brief EnemyAIDirectorのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "SingletonComponent.h"
#include "EnemyAIDirector.h"

#include "FactionCoordinator.h"
#include "EnemyBase.h"

#include "MaruUtility.h"

namespace basecross {

	ex_weak_ptr<Enemy::AIDirector> maru::SingletonComponent<Enemy::AIDirector>::sm_instance = nullptr;

	namespace Enemy {

		AIDirector::AIDirector(const std::shared_ptr<GameObject>& objPtr)
			: maru::SingletonComponent<AIDirector>(objPtr)
		{}

		void AIDirector::OnLateStart() {
			SetEnemys();
		}

		void AIDirector::SetEnemys() {
			m_enemys = maru::Utility::FindWeakPtrComponents<EnemyBase>();
		}

		std::shared_ptr<FactionCoordinator> AIDirector::AddFaction() {
			auto coordinator = std::make_shared<FactionCoordinator>(GetThis<AIDirector>());
			m_factionCoordinators.push_back(coordinator);

			return coordinator;
		}

		bool AIDirector::RemoveFaction(const std::shared_ptr<FactionCoordinator>& removeCoordinator) {
			return maru::Utility::RemoveVec(m_factionCoordinators, removeCoordinator);
		}

		//アクセッサ--------------------------------------------------------------------------------------------------

	}
}