/*!
@file OpenDataHandler.h
@brief OpenDataHandlerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class NavGraphNode;
	class AstarGraph;
	class Heuristic;

	//--------------------------------------------------------------------------------------
	///	オープンデータ
	//--------------------------------------------------------------------------------------
	struct OpenData {
		std::weak_ptr<NavGraphNode> parent;	//自分の手前のノード
		std::weak_ptr<NavGraphNode> node;	//ノードのポインタ
		float range;            //実距離
		float heuristic;        //ヒュースリック距離
		bool isActive = true;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		OpenData();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="node">ノード</param>
		/// <param name="range">実距離</param>
		/// <param name="heuristic">ヒュースリック距離</param>
		OpenData(const std::shared_ptr<NavGraphNode>& parent, const std::shared_ptr<NavGraphNode>& node, const float range, const float heuristic);

		/// <summary>
		/// 実距離とヒュースリック距離の合計を返す
		/// </summary>
		/// <returns>合計値</returns>
		float GetSumRange() const;

		bool operator ==(const OpenData& data) const;
	};

	//--------------------------------------------------------------------------------------
	///	オープンデータを扱う者。
	//--------------------------------------------------------------------------------------
	class OpenDataHandler
	{
		using DataPtrList = std::list<std::shared_ptr<OpenData>>;

		std::shared_ptr<Heuristic> m_heuristic;	//ヒュースリックの数値を計算するクラス。

		std::stack<std::shared_ptr<NavGraphNode>> m_route;	//生成したルート

	public:
		OpenDataHandler();

		virtual ~OpenDataHandler() = default;
	
	private:

		/// <summary>
		/// オープンデータを生成するための基準となるオープンデータを検索(一番期待値の高いノードを取得する。)
		/// </summary>
		/// <returns>オープンデータを生成するための開始ノード</returns>
		std::shared_ptr<OpenData> FindSearchBaseOpenData(DataPtrList& openDataList);

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
			const std::shared_ptr<AstarGraph>& graph,
			const std::shared_ptr<NavGraphNode>& startNode,
			const std::shared_ptr<NavGraphNode>& targetNode
		);

		/// <summary>
		/// ルートの生成
		/// </summary>
		/// <param name="openDataList">オープンデータリスト</param>
		/// <param name="targetNode">目標ノード</param>
		/// <returns>検索が成功したらtrue</returns>
		bool CreateRoute(const DataPtrList& openDataList, const std::shared_ptr<NavGraphNode>& targetNode);

		/// <summary>
		/// オープンデータの取得
		/// </summary>
		/// <param name="openDataList">オープンデータリスト</param>
		/// <param name="node">確認したいノード</param>
		/// <returns>同じデータがあるならそのデータを返す</returns>
		std::shared_ptr<OpenData> FindSomeOpenData(const DataPtrList& dataList, const std::shared_ptr<NavGraphNode>& node);

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
			const std::shared_ptr<NavGraphNode>& startNode, 
			const std::shared_ptr<NavGraphNode>& targetNode,
			const std::shared_ptr<AstarGraph>& graph
		);

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ルートの取得
		/// </summary>
		std::stack<std::shared_ptr<NavGraphNode>> GetRoute();

	};

}