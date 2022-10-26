/*!
@file MoveAstar.h
@brief MoveAstarなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/TaskList/TaskList.h"

namespace basecross {

	class TargetManager;
	class VelocityManager;
	class SelfAstarNodeController;

	template<class T>
	class TaskList;

	struct Task_MovePositions_Parametor;

	namespace Enemy {
		class EnemyBase;
	}

	namespace Task {

		enum class TaskEnum {
			Move,
		};

		//--------------------------------------------------------------------------------------
		///	ターゲットの近くまでAstarを利用して移動するタスクパラメータ
		//--------------------------------------------------------------------------------------
		struct MoveAstar_Parametor {
			std::shared_ptr<Task_MovePositions_Parametor> movePositionsParam;

			MoveAstar_Parametor();
		};

		//--------------------------------------------------------------------------------------
		///	ターゲットの近くまでAstarを利用して移動するタスク
		//--------------------------------------------------------------------------------------
		class MoveAstar : public basecross::TaskNodeBase<Enemy::EnemyBase>
		{
		public:
			using Parametor = MoveAstar_Parametor;

		private:
			const Parametor* m_param;						//パラメータ

			std::unique_ptr<TaskList<TaskEnum>> m_taskList;	//タスクリスト

			std::queue<int> m_areaRoute;					//確認するエリアルート

			std::weak_ptr<Transform> m_transform;			
			std::weak_ptr<TargetManager> m_targetManager;	
			std::weak_ptr<VelocityManager> m_velocityManager;
			std::weak_ptr<SelfAstarNodeController> m_selfAstarNodeController;

			bool m_isInitializeSearch;	//初回サーチ限定
			bool m_isSearchRoute;		//ルートを検索中かどうか
			//static std::mutex m_mtx;	//ミューテックス

		public:
			MoveAstar(const std::shared_ptr<Enemy::EnemyBase>& owner, const Parametor* paramPtr);

			virtual ~MoveAstar() = default;

			void OnStart() override;

			bool OnUpdate() override;

			void OnExit() override;

		private:

			void DefineTask();

			void SelectTask();

			void NextRoute();

			/// <summary>
			/// エリア間のルートを計算する。
			/// </summary>
			/// <returns>エリア間のルート</returns>
			std::queue<int> CalculateMoveAreaRouteQueue();

			/// <summary>
			/// 徘徊する場所の配列を取得
			/// </summary>
			/// <returns>徘徊する場所の配列</returns>
			std::vector<Vec3> CalculateMovePositions();

			/// <summary>
			/// 徘徊移動先の目的地を取得
			/// </summary>
			/// <returns>徘徊移動先を取得</returns>
			Vec3 CalculateMoveTargetPosition();

			void SetIsSearchRoute(const bool isSearchRoute) noexcept { m_isSearchRoute = isSearchRoute; }

			bool IsSearchRoute() const noexcept { return m_isSearchRoute; }

		public:

			//--------------------------------------------------------------------------------------
			///	アクセッサ
			//--------------------------------------------------------------------------------------

			bool IsEnd() const;
		};


	}
}