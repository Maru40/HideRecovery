
/*!
@file FactionCoordinator.cpp
@brief FactionCoordinatorのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "FactionCoordinator.h"

#include "SingletonComponent.h"
#include "EnemyAIDirector.h"

namespace basecross {

	namespace Enemy {

		FactionCoordinator::FactionCoordinator(const std::shared_ptr<AIDirector>& director)
			: m_director(director)
		{}

		void FactionCoordinator::OnStart() {

		}

		void FactionCoordinator::OnUpdate() {
			
		}

		void FactionCoordinator::OnExit() {

		}

	}
}