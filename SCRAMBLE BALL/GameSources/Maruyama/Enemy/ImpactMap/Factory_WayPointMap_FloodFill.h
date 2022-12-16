
/*!
@file Factory_WayPointMap_FloodFill.h
@brief Factory_WayPointMap_FloodFillなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Patch/CommonStructs/RectData.h"

//#include "Maruyama/Patch/Graphs/SparseGraph_Ex.h"
#include "Maruyama/Enemy/Astar/AstarNode.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	//class AstarGraph;
	//class GraphAstar;

	namespace maru {
		class NodeBase;
		class EdgeBase;
		class AstarNode;

		//template<class NodeType, class EdgeType>
		//class SparseGraph;

		template<class NodeType, class EdgeType,
			std::enable_if_t<
				std::is_base_of_v<NodeBase, NodeType>&&		//NodeTypeがNodeBaseを継承していることを保証する
				std::is_base_of_v<EdgeBase, EdgeType>,		//EdgeTypeがEdgeBaseを継承していることを保証する
			std::nullptr_t
		> = nullptr>
		class SparseGraph;

		//--------------------------------------------------------------------------------------
		///	Factory_影響マップ_フラッドフィルアルゴリズム
		//--------------------------------------------------------------------------------------
		class Factory_WayPointMap_FloodFill
		{
		public:
			//usingディレクティブ
			using GraphType = SparseGraph<AstarNode, EdgeBase>;

			/// <summary>
			/// 八方向の進行タイプ
			/// </summary>
			enum class DirectionType {
				Right,
				RightForward,
				RightBack,
				Left,
				LeftForward,
				LeftBack,
				Foward,
				Back,
			};

			/// <summary>
			/// 方向タイプ別データ
			/// </summary>
			struct DataByDirectionType {
				Vec3 direction;	//方向
				int plusIndex;	//加算するノードインデックス
			};

			/// <summary>
			/// 生成用のデータ
			/// </summary>
			struct OpenData
			{
				std::weak_ptr<AstarNode> parentNode;	//自分の前のノード
				std::shared_ptr<AstarNode> selfNode;	//自分自身のノード
				bool isActive;							//ノードが生きているかどうか

				OpenData(
					const std::shared_ptr<AstarNode>& parentNode,
					const std::shared_ptr<AstarNode>& selfNode	
				);
			};

			/// <summary>
			/// パラメータ
			/// </summary>
			struct Parametor {
				float intervalRange = 5.0f;		//ノードの間隔距離
				maru::Rect rect;				//四角データ
				float createHeight = 0.5f;		//高さ設定
			};

		private:
			std::weak_ptr<Stage> m_stage;		//ステージ

			mutable std::mutex m_mutex{};		//ミューテックス

			std::queue<std::shared_ptr<OpenData>> m_openDataQueue;				//オープンデータキュー
			std::unordered_map<DirectionType, int> m_plusIndexMapByDirection;	//方向別の加算するインデックス数

		public:
			Factory_WayPointMap_FloodFill(const std::shared_ptr<Stage>& stage);

		private:
			/// <summary>
			/// 方向データに合わせたインデックスの上限を設定する。
			/// </summary>
			/// <param name="parametor">パラメータ</param>
			/// <returns>方向データに合わせたインデックスの上限を設定する</returns>
			std::unordered_map<DirectionType, int> SettingIndexByDirection(const Parametor& parametor);

			/// <summary>
			/// そのWayPointが生成できるかどうかを判断する。
			/// </summary>
			/// <param name="startPosition">開始位置</param>
			/// <param name="targetPosition">生成したい位置</param>
			/// <param name="parametor">生成パラメータ</param>
			bool IsCreate(
				const std::shared_ptr<OpenData>& openData, 
				const std::shared_ptr<GraphType>& graph,
				const Parametor& parametor
			);

			/// <summary>
			/// ウェイポイントの複数生成
			/// </summary>
			/// <param name="startPosition">開始位置</param>
			/// <param name="parametor">生成パラメータ</param>
			void CreateWayPoints(
				const std::shared_ptr<OpenData>& parentOpenData,
				const std::shared_ptr<GraphType>& graph,
				const Parametor& parametor
			);

			/// <summary>
			/// インデックスを計算して返す
			/// </summary>
			/// <param name="parentOpenData">親となるオープンデータ</param>
			/// <param name="directionType">生成する方向データ</param>
			/// <returns></returns>
			int CalculateIndex(const std::shared_ptr<OpenData>& parentOpenData, const DirectionType directionType) const;

			/// <summary>
			/// 八方向のOpenDataを生成する。
			/// </summary>
			/// <param name="parentOpenData">親となるオープンデータ</param>
			/// <param name="parametor">パラメータ</param>
			/// <returns>八方向のOpenDataを生成する</returns>
			std::vector<std::shared_ptr<OpenData>> CreateChildrenOpenDatas(
				const std::shared_ptr<OpenData>& parentOpenData,
				const Parametor& parametor
			);

		public:
			/// <summary>
			/// ウェイポイントの生成
			/// </summary>
			/// <param name="startPosition">開始位置</param>
			/// <param name="graph">生成したいグラフ</param>
			/// <param name="parametor">生成パラメータ</param>
			void AddWayPointMap(
				const std::shared_ptr<GraphType>& graph,
				const Parametor& parametor
			);

			std::shared_ptr<Stage> GetStage() const noexcept { return m_stage.lock(); }
		};

	}
}