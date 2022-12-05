/*!
@file MoveAstar.h
@brief MoveAstarなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/TaskList/TaskList.h"

//#include "Maruyama/Enemy/Astar/GraphAstar.h"
//#include "Maruyama/Enemy/Astar/AstarEdge.h"

namespace basecross {

	class TargetManager;
	class VelocityManager;
	class SelfAstarNodeController;
	class NavGraphNode;
	class OpenDataHandler;
	class AstarGraph;

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
		///	threadを管理するデータ
		//--------------------------------------------------------------------------------------
		struct ThreadData
		{
			bool m_isRunning;
			std::thread m_thread;
			std::function<void()> startEvent;
			std::function<void()> exitEvent;

			ThreadData(std::thread& newThread);

		private:
			void StartEvent() { if (startEvent) { startEvent(); } }

		public:
			/// <summary>
			/// 終了イベント(内部で呼ぶと排他制御がままならなくなるためこうしている。)
			/// </summary>
			void ExitEvent() { if (exitEvent) { exitEvent(); } }
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

			std::shared_ptr<OpenDataHandler> m_areaOpenDataHandler;
			std::shared_ptr<OpenDataHandler> m_openDataHandler;

			bool m_isInitializeSearch;	//初回サーチ限定
			bool m_isSearchRoute;		//ルートを検索中かどうか
			//static std::mutex m_mtx;	//ミューテックス
			std::mutex m_mtx;

			std::thread::id m_currentThreadID;	//現在登録中のマルチタスク

			std::shared_ptr<ThreadData> m_threadData;

		public:
			MoveAstar(const std::shared_ptr<Enemy::EnemyBase>& owner, const Parametor* paramPtr);

			virtual ~MoveAstar() = default;

			void OnStart() override;

			bool OnUpdate() override;

			void OnExit() override;

		private:

			void DefineTask();

			void SelectTask();

			//次の場所の検索を開始
			void StartThread_NextRoute();

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

			/// <summary>
			/// 徘徊先の目的ノードを取得
			/// </summary>
			std::shared_ptr<NavGraphNode> CalculateMoveTargetNode();

			//エリアIndexの取得(本来はAstar系として別で実装)
			std::vector<int> SearchAreaIndices(const Vec3& startPosition, const Vec3& targetPosition);

			//ルートの取得(本来はAstar系として別で実装)
			std::vector<Vec3> CalculateRoutePositions(
				const std::shared_ptr<NavGraphNode>& selfNode,
				const std::shared_ptr<NavGraphNode>& targetNode,
				const int areaIndex,
				const int targetAreaIndex
			);

			//Astar検索のスタート(本来はAstar系として別で実装)
			std::stack<std::weak_ptr<NavGraphNode>> SearchAstarStart(
				const std::shared_ptr<NavGraphNode>& selfNode,
				const std::shared_ptr<NavGraphNode>& targetNode,
				const std::shared_ptr<AstarGraph>& graph,
				const int areaIndex = -1
			);

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