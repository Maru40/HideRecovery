/*!
@file GraphAstar.h
@brief GraphAstarなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class NavGraphNode;
	class AstarEdge;
	class Heuristic;

	template<class node_type, class edge_type>
	class SparseGraph;

	//--------------------------------------------------------------------------------------
	///	オープンデータ
	//--------------------------------------------------------------------------------------
	struct OpenData {
		ex_weak_ptr<NavGraphNode> node;  //ノードのポインタ
		float range;            //実距離
		float heuristic;        //ヒュースリック距離
		bool isActive = true;   //Active状態かどうか

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
		OpenData(const std::shared_ptr<NavGraphNode>& node, const float& range, const float& heuristic);

		/// <summary>
		/// 実距離とヒュースリック距離の合計を返す
		/// </summary>
		/// <returns>合計値</returns>
		float GetSumRange() const;
	};


	//--------------------------------------------------------------------------------------
	///	Astarの管理をするグラフ
	//--------------------------------------------------------------------------------------
	class GraphAstar
	{
	public:
		using GraphType = SparseGraph<NavGraphNode, AstarEdge>;
		using GraphMap = std::unordered_map<int, std::shared_ptr<GraphType>>;

	private:

		std::shared_ptr<GraphType> m_baseGraph = nullptr;	//グラフのデータ
		GraphMap m_graphMap;								//グラフのエリアインデックスごとに分けたマップ
		std::shared_ptr<Heuristic> m_heuristic = nullptr;	//ヒュースリックの数値を計算するクラス。

		std::map<int, OpenData> m_openDataMap;             //オープンデータのMap
		std::stack<std::shared_ptr<NavGraphNode>> m_route; //生成したルート

		bool m_isRouteEnd = true;        //ルートの最後に来たかどうか？

		std::vector<int> m_debugIndices; //デバッグ用のインデックス

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="graph">グラフデータ</param>
		GraphAstar(const std::shared_ptr<GraphType>& graph);

		/// <summary>
		/// 最短ノードの中で今欲しいノードの場所を取得する。
		/// </summary>
		/// <param name="objPtr">現在この処理を行っているオブジェクト</param>
		/// <returns>現在向かうべきノードのポジション</returns>
		Vec3 CalucTargetNode(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// 自分ノードとエッジから、どのルートが一番近いか検索
		/// </summary>
		/// <param name="self">開始のオブジェクト</param>
		/// <param name="target">ターゲットとなるオブジェクト</param>
		void SearchAstarStart(const std::shared_ptr<GameObject>& self, const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// 自分ノードとエッジから、どのルートが一番近いか検索
		/// </summary>
		/// <param name="self">開始のオブジェクト</param>
		/// <param name="targetPos">ターゲットのポジション</param>
		void SearchAstarStart(const std::shared_ptr<GameObject>& self, const Vec3& targetPos);

		/// <summary>
		/// 自分ノードとエッジから、どのルートが一番近いか検索
		/// </summary>
		/// <param name="selfPos">開始のポジション</param>
		/// <param name="targetPos">ターゲットのポジション</param>
		void SearchAstarStart(const Vec3& selfPos, const Vec3& targetPos);

		/// <summary>
		/// ランダムなルートを計測
		/// </summary>
		/// <param name="selfPosition">自分自身のノード</param>
		/// <returns>生成したルートを返す</returns>
		std::vector<Vec3> CalculateRandomRoute(const Vec3& selfPosition);

	private:
		/// <summary>
		/// Route構築開始
		/// </summary>
		/// <param name="selfNearNode">自分自身のノード</param>
		/// <param name="targetNearNode">たどり着きたいノード</param>
		void SearchAstarStart(const std::shared_ptr<NavGraphNode>& selfNearNode, const std::shared_ptr<NavGraphNode>& targetNearNode);

		/// <summary>
		/// オープンデータ生成の基準となるノードを生成。
		/// </summary>
		/// <param name="initialNode">初期ノード</param>
		/// <returns>基準のノード</returns>
		std::shared_ptr<NavGraphNode> CalculateCreateOpenDataBaseNode(const std::shared_ptr<NavGraphNode>& initialNode);

		/// <summary>
		/// オープンデータの生成
		/// </summary>
		/// <param name="baseNode">基準となるノード</param>
		/// <param name="graph">グラフデータ</param>
		/// <returns></returns>
		bool CreateOpenData(const ex_weak_ptr<NavGraphNode>& baseNode, const std::shared_ptr<GraphType>& graph);

		/// <summary>
		/// 最短ルートの生成
		/// </summary>
		/// <param name="initialNode">初期ノード</param>
		/// <param name="targetNode">たどり着きたいノード</param>
		/// <param name="graph">グラフデータ</param>
		void CreateRoute(
			const std::shared_ptr<NavGraphNode>& initialNode, 
			const std::shared_ptr<NavGraphNode>& targetNode,
			const std::shared_ptr<GraphType>& graph
		);

		/// <summary>
		/// ループして探索経路を測る。
		/// </summary>
		/// <param name="initialNode">初期スタートノード</param>
		void LoopSearchAstar(const std::shared_ptr<NavGraphNode>& initialNode, const std::shared_ptr<NavGraphNode>& targetNode);

	public:
		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// グラフのコピーを返す
		/// </summary>
		/// <returns>グラフのコピー</returns>
		std::shared_ptr<GraphType> CreateCopyGraph();

		/// <summary>
		/// 検索したルートの最後まで来たらtrue
		/// </summary>
		/// <returns>最後まで来たらtrue</returns>
		bool IsRouteEnd() {
			if (m_route.size() == 0) {
				m_isRouteEnd = true;
			}

			return m_isRouteEnd;
		}

		/// <summary>
		/// グラフの取得
		/// </summary>
		/// <returns>グラフ</returns>
		const std::shared_ptr<const GraphType> GetGraph() const {
			return m_baseGraph;
		}

		/// <summary>
		/// エリアごとに分けたグラフの取得
		/// </summary>
		/// <param name="areaIndex">エリアインデックス</param>
		/// <returns>エリアごとに分けたグラフ</returns>
		const std::shared_ptr<const GraphType> GetGraph(const int areaIndex) const {
			return m_graphMap.at(areaIndex);
		}

		/// <summary>
		/// グラフが登録されているかどうか
		/// </summary>
		/// <param name="areaIndex">エリアインデックス</param>
		/// <returns>グラフ</returns>
		bool HasGraph(const int areaIndex) const {
			return m_graphMap.count(areaIndex) != 0;	//0でないならtrue
		}

		/// <summary>
		/// ルートの取得
		/// </summary>
		/// <returns>ルート</returns>
		const std::stack<std::shared_ptr<NavGraphNode>> GetRoute() const {
			return m_route;
		}

		/// <summary>
		/// ルートをベクター配列にして返す。
		/// </summary>
		/// <returns></returns>
		std::vector<std::shared_ptr<NavGraphNode>> GetRouteVec() const;

		/// <summary>
		/// 確保したルートのポジション群
		/// </summary>
		/// <returns>ルートのポジション群を返す</returns>
		std::vector<Vec3> GetRoutePositions() const;

		/// <summary>
		/// ノードの追加
		/// </summary>
		/// <param name="position">追加したいノードのポジション</param>
		/// <returns>追加したノードのインデックス</returns>
		//int AddNode(const Vec3& position);

		template<class... Ts,
			std::enable_if_t<std::is_constructible_v<NavGraphNode, int, Ts...>, std::nullptr_t> = nullptr>
		int AddNode(Ts&&... params) {
			//ノードの生成
			auto index = m_baseGraph->GetNextFreeNodeIndex();
			auto newNode = std::make_shared<NavGraphNode>(index, params...);

			m_baseGraph->AddNode(newNode);	//ベースグラフに登録
			AddAreaGraphNode(newNode);		//エリアインデックスごとのマップに登録する。
			
			return index++;
		}

		/// <summary>
		/// エリアインデックスごとに分けたエリアマップに登録する。
		/// </summary>
		/// <param name="node">登録するノード</param>
		void AddAreaGraphNode(const std::shared_ptr<NavGraphNode>& node);

		/// <summary>
		/// ノードの追加
		/// </summary>
		/// <param name="position">追加したいノードのポジション</param>
		/// <param name="obstacleObjs">障害物対象のオブジェクト</param>
		/// <param name="excluteObjs">障害物から除く対象のオブジェクト</param>
		/// <returns>追加したノードのインデックス</returns>
		int AddNode(const Vec3& position,
			const vector<shared_ptr<GameObject>>& obstacleObjs, const vector<shared_ptr<GameObject>>& excluteObjs);

		/// <summary>
		/// ノードの削除
		/// </summary>
		/// <param name="index">削除したいノードのインデックス</param>
		void RemoveNode(const int& index);

		/// <summary>
		/// ノードの親オブジェクトを設定する。
		/// </summary>
		/// <param name="parent">ペアレントにするオブジェクト</param>
		void SetNodesParent(const std::shared_ptr<GameObject>& parent);

		/// <summary>
		/// エッジの追加
		/// </summary>
		void AddEdges();

		/// <summary>
		/// ノードの全てクリア
		/// </summary>
		void ClearNodes();

		/// <summary>
		/// エッジの全てクリア
		/// </summary>
		void ClearEdges();

		/// <summary>
		/// Astarのリセット
		/// </summary>
		void ResetAstar(); 

		/// <summary>
		/// 全てリセット
		/// </summary>
		void ResetAll();


	private:
		//デバッグ処理-------------------------------------------------------

		/// <summary>
		/// デバッグ表示
		/// </summary>
		void DebugDraw();

	};
}

//endbaecross