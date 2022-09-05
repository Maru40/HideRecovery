
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
#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "MaruUtility.h"

namespace basecross {

	std::weak_ptr<Enemy::AIDirector> maru::SingletonComponent<Enemy::AIDirector>::sm_instance;

	namespace Enemy {

		AIDirector::AIDirector(const std::shared_ptr<GameObject>& objPtr)
			: maru::SingletonComponent<AIDirector>(objPtr)
		{}

		void AIDirector::OnLateStart() {
			SettingStartAllEnemys();

			//仮で全員一つのファクションにアサイン
			auto faction = CreateFaction(m_enemys);
		}

		void AIDirector::OnUpdate() {
			//グループ管理の更新
			for (auto& faction : m_factionCoordinators) {
				faction->OnUpdate();
			}
		}

		void AIDirector::SettingStartAllEnemys() {
			m_enemys = maru::Utility::FindWeakPtrComponents<EnemyBase>();
		}

		std::shared_ptr<FactionCoordinator> AIDirector::CreateFaction(const std::vector<std::weak_ptr<EnemyBase>>& assignMembers) {
			auto coordinator = std::make_shared<FactionCoordinator>(GetThis<AIDirector>(), assignMembers);
			coordinator->OnStart();
			m_factionCoordinators.push_back(coordinator);

			return coordinator;
		}

		bool AIDirector::RemoveFaction(const std::shared_ptr<FactionCoordinator>& removeCoordinator) {
			removeCoordinator->OnExit();
			return maru::Utility::RemoveVec(m_factionCoordinators, removeCoordinator);
		}

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		void AIDirector::AddEnemy(const std::shared_ptr<EnemyBase>& enemy) {
			m_enemys.push_back(enemy);
		}

		std::shared_ptr<FactionCoordinator> AIDirector::GetFactionCoordinator(const int index) const {
			if (m_factionCoordinators.size() <= index) { //サイズが小さかったら配列オーバー
				return nullptr;
			}

			return m_factionCoordinators[index];
		}
	}
}