
/*!
@file PatrolCoordinator.cpp
@brief PatrolCoordinator�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "PatrolCoordinator.h"

#include "EnemyBase.h"
#include "FactionCoordinator.h"

#include "MaruUtility.h"

namespace basecross {

	namespace Enemy {

		PatrolCoordinator::PatrolCoordinator(const std::shared_ptr<FactionCoordinator>& owner):
			HereOwnerCoordinatorBase(owner)
		{}

	}
}