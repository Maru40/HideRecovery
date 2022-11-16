/*!
@file GraphAstar.h
@brief GraphAstarなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "AstarGraph.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class NavGraphNode;
	class AstarEdge;
	class Heuristic;

	template<class node_type, class edge_type>
	class SparseGraph;
	class AstarGraph;

	//--------------------------------------------------------------------------------------
	///	Astarの管理をするグラフ
	//--------------------------------------------------------------------------------------
	class GraphAstar
	{
	public:
		using GraphType = AstarGraph;
		using GraphMap = std::unordered_map<int, std::shared_ptr<GraphType>>;

	private:

		std::shared_ptr<GraphType> m_areaGraph = nullptr;	//エリアインデックス用のグラフ生成
		std::shared_ptr<GraphType> m_baseGraph = nullptr;	//グラフのデータ(エリア内のグラフデータ)
		GraphMap m_graphMap;								//グラフのエリアインデックスごとに分けたマップ

		std::stack<std::weak_ptr<NavGraphNode>> m_route;	//生成したルート

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="graph">グラフデータ</param>
		GraphAstar(const std::shared_ptr<GraphType>& graph);

		virtual ~GraphAstar() = default;

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
		/// 自分ノードとエッジから、どのルートが一番近いか検索
		/// </summary>
		/// <param name="selfPos">開始のポジション</param>
		/// <param name="targetPos">ターゲットのポジション</param>
		/// <param name="areaIndex">エリアインデックス</param>
		void SearchAstarStart(const Vec3& selfPosition, const Vec3& targetPosition, const int areaIndex, const int targetAreaIndex = -1);

		void SearchAstarStart(
			const std::shared_ptr<NavGraphNode>& selfNode,
			const Vec3& targetPosition,
			const int areaIndex,
			const int targetAreaIndex = -1
		);

		void SearchAstarStart(
			const std::shared_ptr<NavGraphNode>& selfNode,
			const std::shared_ptr<NavGraphNode>& targetNode,
			const int areaIndex,
			const int targetAreaIndex = -1
		);

		/// <summary>
		/// ランダムなルートを計測
		/// </summary>
		/// <param name="selfPosition">自分自身のノード</param>
		/// <returns>生成したルートを返す</returns>
		std::vector<Vec3> CalculateRandomRoute(const Vec3& selfPosition);

		/// <summary>
		/// ターゲットへのエリア単位の大まかなルートを検索
		/// </summary>
		std::vector<int> SearchAreaRouteIndices(const Vec3& selfPosition, const Vec3& targetPosition);

	private:
		/// <summary>
		/// Route構築開始
		/// </summary>
		/// <param name="selfNearNode">自分自身のノード</param>
		/// <param name="targetNearNode">たどり着きたいノード</param>
		void SearchAstarStart(
			const std::shared_ptr<NavGraphNode>& selfNearNode, 
			const std::shared_ptr<NavGraphNode>& targetNearNode,
			const std::shared_ptr<GraphType>& graph,
			const int targetAreaIndex = -1
		);

	public:

		/// <summary>
		/// それぞれのグラフの中心位置を設定する。
		/// (現在はImpactMapから生成しているが、他の方法を検討中)
		/// </summary>
		void CreateAreaAstarGraph();

		/// <summary>
		/// 一番近いエリアのインデックスを取得する。
		/// </summary>
		/// <param name="position">自分のポジション</param>
		/// <returns>一番近いエリアのインデックス</returns>
		int SearchNearAreaIndex(const Vec3& position);

		/// <summary>
		/// 一番近いエリアのノードを取得
		/// </summary>
		/// <param name="position">自分のポジション</param>
		/// <returns>一番近いエリアのポジション</returns>
		std::shared_ptr<NavGraphNode> SearchNearAreaNode(const Vec3& position);

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// グラフのコピーを返す
		/// </summary>
		/// <returns>グラフのコピー</returns>
		std::shared_ptr<GraphType> CreateCopyGraph();

		/// <summary>
		/// グラフのコピーを返す
		/// </summary>
		/// <param name="areaIndex">コピーしたいグラフ</param>
		/// <returns>グラフのコピー</returns>
		std::shared_ptr<GraphType> CreateCopyGraph(const std::shared_ptr<GraphType>& baseGraph);

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
			if (m_graphMap.count(areaIndex) == 0) {
				return nullptr;
			}

			return m_graphMap.at(areaIndex);
		}

		const std::shared_ptr<const GraphType> GetAreaGraph() const { 
			return m_areaGraph;
		}

		/// <summary>
		/// 書き換え可能な状態のエリアグラフを取得
		/// </summary>
		/// <returns></returns>
		const std::shared_ptr<GraphType> GetReWiritingAreaGraph() const {
			return m_areaGraph;
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
		const std::stack<std::weak_ptr<NavGraphNode>> GetRoute() const {
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
		/// エリアごとのルートをエリアインデックス配列で受け取る。
		/// </summary>
		std::vector<int> GetRouteAreaIndex() const;

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

		void RemoveNode(const std::shared_ptr<NavGraphNode>& node);

		/// <summary>
		/// ノードの親オブジェクトを設定する。
		/// </summary>
		/// <param name="parent">ペアレントにするオブジェクト</param>
		void SetNodesParent(const std::shared_ptr<GameObject>& parent);

		/// <summary>
		/// エッジの追加
		/// </summary>
		void AddEdges(const bool isRayHit = true);

		/// <summary>
		/// エッジの追加
		/// </summary>
		/// <param name="graph">追加したいグラフ</param>
		void AddEdges(const std::shared_ptr<GraphAstar::GraphType>& graph, const bool isRayHit = true);

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
		void ResetAllAstar();

	private:
		//デバッグ処理-------------------------------------------------------

		/// <summary>
		/// デバッグでノードのインデックスを表示する。
		/// </summary>
		/// <param name="node">表示したいノード</param>
		/// <param name="description">説明文</param>
		void DebugNodeIndexDraw(const std::shared_ptr<NavGraphNode>& node, const std::wstring& description = L"");

		/// <summary>
		/// デバッグで、開始ノードと目標ノードのインデックス表示をする。
		/// </summary>
		void DebugStartAndEndIndexDraw(const std::shared_ptr<NavGraphNode>& startNode, const std::shared_ptr<NavGraphNode>& targetNode);

		/// <summary>
		/// デバッグで、ルートインデックスの表示
		/// </summary>
		void DebugRouteIndexDraw();

	};
}

//endbaecross