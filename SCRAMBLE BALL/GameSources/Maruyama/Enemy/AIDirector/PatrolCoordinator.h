
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

			PatrolCoordinator(const std::shared_ptr<FactionCoordinator>& owner, const std::vector<std::weak_ptr<EnemyBase>>& members);

			~PatrolCoordinator() = default;

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override {}

		private:
			/// <summary>
			/// パトロールする場所のコントロール
			/// </summary>
			void PatrolControl();

			/// <summary>
			/// ターゲット発見の通知を受け取ったときにする処理
			/// </summary>
			void ObserveTuple_FindTarget();
		};

	}
}