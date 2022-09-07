/*!
@file SparseGraph.h
@brief SparseGraph
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Utility/Utility.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	using maru::Utility;

	//--------------------------------------------------------------------------------------
	///	汎用型グラフ
	//--------------------------------------------------------------------------------------
	template<class node_type, class edge_type>
	class SparseGraph
	{
	public:
		//クライアントからグラフで使われているエッジとノードへの簡易なアクセスを可能にする。
		using NodeType = node_type;
		using EdgeType = edge_type;

		using NodeVector = std::vector<std::shared_ptr<node_type>>;
		using EdgeVector = std::vector<std::shared_ptr<edge_type>>;
		using EdgeMapVector = std::map<int, EdgeVector>;

	private:
		//このクラスを構成するノード
		NodeVector m_nodes;

		//隣接エッジリストのベクター
		//(各ノードのインデックスは、そのノードに関連付けられたエッジのリストのキーとなる)
		EdgeMapVector m_edges;

		//このノードは有効グラフかどうか？
		bool m_isDigraph;

		//追加されるべき次のノードインデックス
		int m_nextNodeIndex;

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="digraph">有効かどうか</param>
		SparseGraph(bool digraph) :
			m_nextNodeIndex(0),
			m_isDigraph(digraph)
		{}

		/// <summary>
		/// 指定されたインデックスのノードを返す
		/// </summary>
		/// <param name="index">欲しいノードのインデックス</param>
		/// <returns>指定されたインデクスのノード</returns>
		const std::shared_ptr<NodeType> GetNode(const int& index) const {
			for (const auto& node : m_nodes) {
				if (node->GetIndex() == index) {
					return node;
				}
			}

			return nullptr;
		}

		/// <summary>
		/// 渡された引数番目に保存されているノードの取得
		/// </summary>
		/// <param name="index">欲しい配列のインデックス</param>
		/// <returns>ノード</returns>
		const std::shared_ptr<NodeType> GetArrayNumberNode(const int& index) {
			//インデックスオーバーならnullptrを返す
			return index < m_nodes.size() ? m_nodes[index] : nullptr;
		}

		/// <summary>
		/// 全てのノードを渡す
		/// </summary>
		/// <returns>全てのノード</returns>
		const std::vector<std::shared_ptr<NodeType>>& GetNodes() const {
			return m_nodes;
		}

		/// <summary>
		/// ノードの中身をコピーして渡す
		/// </summary>
		std::vector<std::shared_ptr<NodeType>> GetNodesCopy() const {
			std::vector<std::shared_ptr<NodeType>> results;

			for (const auto& node : m_nodes) {
				results.push_back(std::make_shared<NodeType>(*(node.get())));
			}

			return results;
		}

		/// <summary>
		/// ノードが入っているインデックス番号の取得
		/// </summary>
		/// <param name="index">取得したいインデックス</param>
		/// <returns>ノードが入っているインデックス番号の取得</returns>
		int GetCurrentNodeArrayIndex(const int& index) {
			for (int i = 0; i < m_nodes.size(); i++) {
				if (m_nodes[i]->GetIndex() == index) {
					return i;
				}
			}

			return 0;
		}

		/// <summary>
		/// エッジの参照を得る
		/// </summary>
		/// <param name="from"></param>
		/// <param name="back"></param>
		/// <returns>エッジの参照</returns>
		const std::shared_ptr<EdgeType>& GetEdge(const int& from, const int& to) const {
			for (auto edge : m_edges[from]) {
				if (edge->GetTo() == to) {
					return edge;
				}
			}

			return nullptr;
		}

		/// <summary>
		/// 与えられた引数から始まるエッジリストを全て取得
		/// </summary>
		/// <param name="from">欲しいエッジリストのインデックス</param>
		/// <returns>エッジリスト</returns>
		const std::vector<std::shared_ptr<EdgeType>> GetEdges(const int& from) const {
			if (m_edges.count(from) == 0) {
				return std::vector<std::shared_ptr<EdgeType>>();
			}
			return m_edges.at(from);
		}

		/// <summary>
		/// エッジの連想配列を全て取得
		/// </summary>
		/// <returns>エッジの連想配列全て</returns>
		const EdgeMapVector& GetEdgesMap() const {
			return m_edges;
		}

		/// <summary>
		/// エッジの連想配列のコピーを渡す。
		/// </summary>
		/// <returns>エッジの連想配列コピー</returns>
		EdgeMapVector GetEdgesMapCopy() const {
			EdgeMapVector results;

			for (const auto& pair : m_edges) {
				results[pair.first] = EdgeVector();
				for (const auto& edge : pair.second) {
					results[pair.first].push_back(std::make_shared<EdgeType>(*(edge.get())));
				}
			}

			return results;
		}

		/// <summary>
		/// エッジの連想配列をセットする。
		/// </summary>
		/// <param name="edgesMap">エッジの連想配列全て</param>
		void SetEdgesMap(const EdgeMapVector& edgesMap) {
			m_edges = edgesMap;
		}

		/// <summary>
		/// 次のフリーノードのインデックスを参照する。
		/// </summary>
		/// <returns>次のフリーノードのインデックス</returns>
		int GetNextFreeNodeIndex() {
			//見つかった場合は別のインデックスにしなければならない
			if (m_edges.find(m_nextNodeIndex) != m_edges.end()) {
				m_nextNodeIndex = CalucNextFreeNodeIndex();
			}

			return m_nextNodeIndex;
		}

		/// <summary>
		/// このグラフのノードを追加して、そのインデックスを返す。
		/// </summary>
		/// <param name="node">追加したいノード</param>
		/// <returns>追加したノードのインデックス</returns>
		int AddNode(const std::shared_ptr<NodeType>& node) {
			m_nodes.push_back(node);
			return m_nextNodeIndex++;
			//return (int)m_nodes.size() - 1;
		}

		/// <summary>
		/// ノードの配列をセット
		/// </summary>
		/// <param name="nodes">セットしたいノードの配列</param>
		void SetNodes(const std::vector<std::shared_ptr<NodeType>>& nodes) {
			m_nodes = nodes;
		}

		/// <summary>
		/// インデックスをinvalid_node_indexに設定することでノードを削除する。
		/// </summary>
		/// <param name="index">削除したいノードのインデックス</param>
		void RemoveNode(const int& index) {
			if (m_edges.find(index) == m_edges.end()) { //見つからなかったら処理をしない。
				return;
			}

			//ノードの削除
			Utility::RemoveVec(m_nodes, m_nodes[index]);

			//そのノードのedgesの削除
			m_edges.erase(index);

			//削除したインデクスが含まれるエッジを削除
			for (int i = 0; i < m_edges.size(); i++) {
				if (m_edges.count(i) == 0) {
					continue;
				}

				for (auto& edge : m_edges[i]) {
					if (edge->GetTo() == index) {  //行先がindexと一緒なら
						RemoveEdge(edge->GetFrom(), edge->GetTo());
					}
				}
			}

			m_nextNodeIndex = index;
		}

		/// <summary>
		/// 全てのノードに親オブジェクトを設定する。
		/// </summary>
		/// <param name="parent">親オブジェクトに設定するオブジェクト</param>
		void SetNodesParent(const std::shared_ptr<GameObject>& parent) {
			for (auto& node : m_nodes) {
				node->SetParent(parent);
			}
		}

		/// <summary>
		/// エッジの追加
		/// </summary>
		/// <param name="edge">追加したいエッジ</param>
		void AddEdge(const std::shared_ptr<EdgeType>& edge) {
			m_edges[edge->GetFrom()].push_back(edge);
		}

		/// <summary>
		/// エッジの削除
		/// </summary>
		/// <param name="from">手前側のノードインデックス</param>
		/// <param name="back">先側のノードインデックス</param>
		void RemoveEdge(const int& from, const int& to) {
			auto& edges = m_edges[from];

			auto iter = edges.begin();
			while (iter != edges.end()) {
				if ((*iter)->GetTo() == to) {
					iter = edges.erase(iter);
					break;
				}

				iter++;
			}
		}

		/// <summary>
		/// 渡されたインデックスが含まれるエッジを全て削除する。
		/// </summary>
		/// <param name="index">削除したいインデックス</param>
		void RemoveEdge(const int& index) {
			vector<std::function<void()>> functions;

			for (const auto& pair : m_edges) {
				for (const auto& edge : pair.second) {
					if (edge->GetTo() == index) {
						functions.push_back([&, pair, index]() { pair.second.erase(index); });
					}
				}
			}

			for (auto& function : functions) {
				function();
			}
		}

		/// <summary>
		/// ノードのクリア
		/// </summary>
		void ClearNodes() {
			m_nodes.clear();
		}

		/// <summary>
		/// エッジのクリア
		/// </summary>
		void ClearEdges() {
			m_edges.clear();
		}

		/// <summary>
		/// このグラフ内のノードの個数を返す。
		/// </summary>
		/// <returns>ノードの個数</returns>
		int GetNumNodes() const {
			return static_cast<int>(m_nodes.size());
		}

		/// <summary>
		/// このグラフ内に存在するアクティブなノードの個数を返す。
		/// </summary>
		/// <returns>アクティブなノードの個数</returns>
		int GetNumActiveNodes() const {
			int num = 0;
			for (auto node : m_nodes) {
				if (node.IsActive()) {
					num++;
				}
			}
			return num;
		}

		/// <summary>
		/// エッジの数を取得
		/// </summary>
		/// <returns>エッジの数</returns>
		int GetNumEdges() const {
			int num = 0;
			for (auto edges : m_edges) {
				num += (int)edges.second.size();
			}

			return num;
		}

		/// <summary>
		/// このグラフが有効かどうかを返す。
		/// </summary>
		/// <returns>有効ならtrue</returns>
		bool IsDigraph() const {
			return m_isDigraph;
		}

		/// <summary>
		/// ノードが存在するかどうかを返す。
		/// </summary>
		/// <returns>ノードが存在するならtrue</returns>
		bool IsEmpty() const {
			return m_nodes.size() == 0 ? true, false;
		}

		/// <summary>
		/// 指定したインデックスを持つノードがこのグラフ内に存在する場合、trueを返す。
		/// </summary>
		/// <param name="index">指定したインデックス</param>
		/// <returns>指定したインデックスを持つノードが存在する場合はtrue</returns>
		bool IsPresent(int index) const {
			for (auto node : m_nodes) {
				if (node->GetIndex() == index) {
					return true;
				}
			}

			return false;
		}

		/// <summary>
		/// 全てのエッジとノードをリセット
		/// </summary>
		void ResetAll() {
			m_nodes.clear();
			m_edges.clear();
		}

	private:

		/// <summary>
		/// 現在空きのあるノードのインデックスを探し出す。
		/// </summary>
		/// <returns>空きのインデックス</returns>
		int CalucNextFreeNodeIndex() {
			int index = (int)m_edges.size();  //ループが最後までいったら最後のノードの次が空きノードになる。
			if (index == 0) {
				return ++index;
			}

			for (int i = 0; i < m_edges.size(); i++) {
				if (m_edges.find(i) == m_edges.end()) {  //endだったらそのノードがないから
					index = i;
					break;
				}
			}

			return index;
		}
	};

}