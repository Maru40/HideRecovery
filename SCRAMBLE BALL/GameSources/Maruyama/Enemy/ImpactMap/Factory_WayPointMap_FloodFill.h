
/*!
@file Factory_WayPointMap_FloodFill.h
@brief Factory_WayPointMap_FloodFillなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Patch/CommonStructs/RectData.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class AstarGraph;
	class GraphAstar;

	namespace maru {
		class NodeBase;
		class EdgeBase;

		template<class NodeType, class EdgeType,
			std::enable_if_t<
				std::is_base_of_v<NodeBase, NodeType>&&		//NodeTypeがNodeBaseを継承していることを保証する
				std::is_base_of_v<EdgeBase, EdgeType>,		//EdgeTypeがEdgeBaseを継承していることを保証する
			std::nullptr_t
		> = nullptr>
		class SparceGraph;

		//--------------------------------------------------------------------------------------
		///	Factory_影響マップ_フラッドフィルアルゴリズム
		//--------------------------------------------------------------------------------------
		class Factory_WayPointMap_FloodFill
		{
		public:
			//usingディレクティブ
			using GraphType = SparceGraph<NodeBase, EdgeBase>;

			struct Parametor {
				float intervalRange = 5.0f;		//ノードの間隔距離
			};

		private:

			std::weak_ptr<Stage> m_stage;		//ステージ

			mutable std::mutex m_mutex{};		//ミューテックス

		public:

			Factory_WayPointMap_FloodFill(const std::shared_ptr<Stage>& stage);

		private:
			/// <summary>
			/// そのWayPointが生成できるかどうかを判断する。
			/// </summary>
			/// <param name="startPosition">開始位置</param>
			/// <param name="targetPosition">生成したい位置</param>
			/// <param name="parametor">生成パラメータ</param>
			bool IsCreate(const Vec3& startPosition, const Vec3& targetPosition, const Parametor& parametor);

			/// <summary>
			/// 開始位置から八方向の中でウェイポイントを生成できる場所を返す。
			/// </summary>
			/// <param name="startPosition">開始位置</param>
			/// <param name="parametor">生成パラメータ</param>
			/// <returns>開始位置から八方向の中でウェイポイントを生成できる場所の配列</returns>
			std::vector<Vec3> CalculationTargetPositions(const Vec3& startPosition, const Parametor& parametor);

			/// <summary>
			/// ウェイポイントの複数生成
			/// </summary>
			/// <param name="startPosition">開始位置</param>
			/// <param name="parametor">生成パラメータ</param>
			void CreateWayPoints(
				const Vec3& startPosition,
				const std::shared_ptr<GraphType>& graph,
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
				const Vec3& baseStartPosition,
				const std::shared_ptr<GraphType>& graph,
				const Parametor& parametor
			);

			std::shared_ptr<Stage> GetStage() const noexcept { return m_stage.lock(); }
		};

	}
}