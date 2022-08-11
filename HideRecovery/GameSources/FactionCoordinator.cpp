
/*!
@file FactionCoordinator.cpp
@brief FactionCoordinator�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "FactionCoordinator.h"

#include "SingletonComponent.h"
#include "EnemyAIDirector.h"

#include "EnemyBase.h"

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

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

	}
}