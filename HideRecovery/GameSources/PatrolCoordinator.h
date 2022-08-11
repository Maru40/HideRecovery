
/*!
@file PatrolCoordinator.h
@brief PatrolCoordinatorなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "CoordinatorBase.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class EnemyBase;
		class FactionCoordinator;

		//--------------------------------------------------------------------------------------
		/// パトロール中の調整者
		//--------------------------------------------------------------------------------------
		class PatrolCoordinator : public HereOwnerCoordinatorBase<FactionCoordinator, EnemyBase>
		{
		public:
			PatrolCoordinator(const std::shared_ptr<FactionCoordinator>& owner);

			~PatrolCoordinator() = default;
		};

	}
}