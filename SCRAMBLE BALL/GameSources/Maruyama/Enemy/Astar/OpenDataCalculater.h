/*!
@file OpenDataCalculater.h
@brief OpenDataCalculaterなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

//#include "Maruyama/StateMachine/NodeBase.h"
//#include "Maruyama/StateMachine/EdgeBase.h"

namespace basecross {

	namespace maru
	{

		//--------------------------------------------------------------------------------------
		///	前方宣言
		//--------------------------------------------------------------------------------------
		class NodeBase;
		class EdgeBase;
		class AstarNode;
		class AstarEdge;

		template<class NodeType, class EdgeType,
			std::enable_if_t<
				std::is_base_of_v<NodeBase, NodeType>&&		//NodeTypeがNodeBaseを継承していることを保証する
				std::is_base_of_v<EdgeBase, EdgeType>,		//EdgeTypeがEdgeBaseを継承していることを保証する
			std::nullptr_t
		>>
		class SparseGraph;

		//--------------------------------------------------------------------------------------
		///	オープンデータ(新規版)
		//--------------------------------------------------------------------------------------
		struct OpenData
		{
			std::weak_ptr<AstarNode> parentNode;	//親ノード
			std::weak_ptr<AstarNode> selfNode;		//自分自身のノード
			float range;		//実コスト
			float heuristic;	//ヒュースリックコスト
			bool isActive;		//有効状態

			OpenData(
				const std::shared_ptr<AstarNode>& parentNode,
				const std::shared_ptr<AstarNode>& selfNode,
				const float range,
				const float heuristic
			);

			/// <summary>
			/// 実コストとヒュースリックコストの合計値を返す
			/// </summary>
			/// <returns>実コストとヒュースリックコストの合計値</returns>
			float GetSumRange() const;

			bool operator ==(const OpenData& data) const;
		};

		//--------------------------------------------------------------------------------------
		///	オープンデータ計算クラス(新規版)
		//--------------------------------------------------------------------------------------
		class OpenDataCalculater
		{
		public:
			using GraphType = SparseGraph<AstarNode, AstarEdge, nullptr>;		//グラフタイプ
			using DataPtrList = std::list<std::shared_ptr<OpenData>>;	//データリスト

		private:
			std::stack<std::weak_ptr<AstarNode>> m_route;

			std::weak_ptr<AstarNode> m_otherAreaNode;		//別のエリアノード(将来的に削除)

		public:
			OpenDataCalculater();

			virtual ~OpenDataCalculater() = default;

		private:
			/// <summary>
			/// ヒュースリックコストを計算して返す
			/// </summary>
			/// <param name="currentNode">現在のノード</param>
			/// <param name="targetNode">目的地のノード</param>
			/// <returns>ヒュースリックコスト</returns>
			float CalculateHeuristicRange(
				const std::shared_ptr<AstarNode> currentNode,
				const std::shared_ptr<AstarNode> targetNode
			) const;

			/// <summary>
			/// オープンデータを生成するための基準となるオープンデータを検索(一番期待値の高いノードを取得する。)
			/// </summary>
			/// <returns>オープンデータを生成するための開始ノード</returns>
			std::shared_ptr<OpenData> FindSearchBaseOpenData(DataPtrList& openDataList);

			/// <summary>
			/// 他のエリアが目標になっているかどうか(まだ未完成)
			/// </summary>
			/// <param name="startNode">開始ノード</param>
			/// <param name="targetAreaIndex">目標エリア</param>
			/// <returns>他のエリアが目標ならtrue</returns>
			bool IsOhterAreaTarget(const std::shared_ptr<AstarNode>& startNode, const int targetAreaIndex);

			/// <summary>
			/// オープンデータを生成する。
			/// </summary>
			/// <param name="baseNode">ベースとなるノード</param>
			/// <param name="graph">グラフの参照</param>
			/// <returns>オープンデータ</returns>
			bool CreateOpenDatas(
				DataPtrList& openDataList,
				DataPtrList& closeDataList,
				const std::shared_ptr<OpenData>& openData,
				const std::shared_ptr<GraphType>& graph,
				const std::shared_ptr<AstarNode>& startNode,
				const std::shared_ptr<AstarNode>& targetNode,
				const int targetAreaIndex = -1
			);

			/// <summary>
			/// ルートの生成
			/// </summary>
			/// <param name="openDataList">オープンデータリスト</param>
			/// <param name="targetNode">目標ノード</param>
			/// <returns>検索が成功したらtrue</returns>
			std::stack<std::weak_ptr<AstarNode>> CreateRoute(const DataPtrList& openDataList, const std::shared_ptr<AstarNode>& targetNode);

			/// <summary>
			/// オープンデータの取得
			/// </summary>
			/// <param name="openDataList">オープンデータリスト</param>
			/// <param name="node">確認したいノード</param>
			/// <returns>同じデータがあるならそのデータを返す</returns>
			std::shared_ptr<OpenData> FindSomeOpenData(const DataPtrList& dataList, const std::shared_ptr<AstarNode>& node);

			/// <summary>
			/// オープンデータの取得
			/// </summary>
			/// <param name="openDataList">オープンデータリスト</param>
			/// <param name="openData">同じデータが存在するか判断</param>
			/// <returns>同じデータがあるならそのデータを返す</returns>
			std::shared_ptr<OpenData> FindSomeOpenData(const DataPtrList& dataList, const std::shared_ptr<OpenData>& openData);

			/// <summary>
			/// 登録されたオープンデータか判断
			/// </summary>
			/// <param name="openDataList">データリスト</param>
			/// <param name="openData">確認したいデータ</param>
			/// <returns>登録されていたらtrue</returns>
			bool IsRegisterData(const DataPtrList& dataList, const std::shared_ptr<OpenData>& openData);

			/// <summary>
			/// オープンデータの追加
			/// </summary>
			/// <param name="openDatas">追加したいオープンデータの配列</param>
			/// <param name="openData">追加したいオープンデータ</param>
			/// <returns></returns>
			bool AddOpenData(DataPtrList& openDataList, DataPtrList& closeDataList, const std::shared_ptr<OpenData>& openData);

		public:

			/// <summary>
			/// Astarを利用した経路探索開始
			/// </summary>
			/// <param name="startNode">開始ノード</param>
			/// <param name="targetNode">目標ノード</param>
			/// <param name="graph">探索に利用するグラフ</param>
			/// <returns>経路探索が完了したらtrue</returns>
			bool StartSearchAstar(
				const std::shared_ptr<AstarNode>& startNode,
				const std::shared_ptr<AstarNode>& targetNode,
				const std::shared_ptr<GraphType>& graph,
				const int targetAreaIndex = -1
			);

			/// <summary>
			/// ルートの取得
			/// </summary>
			std::stack<std::weak_ptr<AstarNode>> GetRoute();

		};
	}

}