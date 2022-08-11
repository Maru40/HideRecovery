
/*!
@file PatrolCoordinator.cpp
@brief PatrolCoordinatorのクラス実体
担当：丸山裕喜
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