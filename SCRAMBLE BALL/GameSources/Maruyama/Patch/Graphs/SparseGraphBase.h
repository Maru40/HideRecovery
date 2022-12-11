
/*!
@file SparseGraphBase.h
@brief SparseGraphBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class NodeBase;
		class EdgeBase;

		//--------------------------------------------------------------------------------------
		///	汎用型グラフ(前回作成したSparseGraphの改良型、将来的にSparseGraphは削除予定)
		//--------------------------------------------------------------------------------------
		template<class NodeType, class EdgeType,
			std::enable_if_t<
				std::is_base_of_v<NodeBase, NodeType>&&		//NodeTypeがNodeBaseを継承していることを保証する
				std::is_base_of_v<EdgeBase, EdgeType>,		//EdgeTypeがEdgeBaseを継承していることを保証する
			std::nullptr_t
		> = nullptr>
		class SparseGraphBase
		{
		public:
			using NodeMap = std::map<int, std::shared_ptr<NodeType>>;
			using EdgeVector = std::vector<std::shared_ptr<EdgeType>>;
			using EdgeVectorMap = std::map<int, EdgeVector>;

		private:
			bool m_isActive = true;		//アクティブかどうか

		protected:
			NodeMap m_nodeMap;			//ノードを格納する配列
			EdgeVectorMap m_edgesMap;	//エッジを格納する配列

		public:
			SparseGraphBase() = default;
			virtual ~SparseGraphBase() = default;

			/// <summary>
			/// 指定したノードの取得
			/// </summary>
			/// <param name="index">ノードインデックス</param>
			/// <returns>指定したノード</returns>
			virtual std::shared_ptr<NodeType> GetNode(const int index) const {
				return m_nodeMap.at(index);
			}

			/// <summary>
			/// ノードの配列を取得
			/// </summary>
			/// <returns>ノード配列</returns>
			virtual NodeMap GetNodes() const {
				return m_nodeMap;
			}

			/// <summary>
			/// 指定したノードを連結するエッジの取得
			/// </summary>
			/// <param name="from">手前のインデックス</param>
			/// <param name="to">先のインデックス</param>
			/// <returns>指定したノードを連結するエッジの取得</returns>
			virtual std::shared_ptr<EdgeType> GetEdge(const int from, const int to) const {
				//存在しなかったらnullptrを返す。
				if (m_edgesMap.count[from] == 0) {
					return nullptr;
				}

				auto edges = m_edgesMap[from];
				for (std::shared_ptr<EdgeBase>& edge : edges) {
					if (edge->GetToType() == to) {
						return edge;
					}
				}

				return nullptr;
			}

			/// <summary>
			/// 指定したノードから伸びるエッジ配列の取得
			/// </summary>
			/// <param name="from">取得したいノード</param>
			/// <returns>指定したノードから伸びるエッジ配列</returns>
			virtual EdgeVector GetEdges(const int from) const {
				if (m_edgesMap.count(from) == 0) {
					return EdgeVector();
				}

				return m_edgesMap.at(from);
			}

			/// <summary>
			/// エッジ連想配列を取得
			/// </summary>
			/// <returns>エッジ連想配列</returns>
			virtual EdgeVectorMap GetEdgesMap() const {
				return m_edgesMap;
			}

			/// <summary>
			/// ノードの追加
			/// </summary>
			/// <param name="index">ノードのタイプ</param>
			/// <param name="node">ノードのポインタ</param>
			virtual void AddNode(const int index, const std::shared_ptr<NodeType>& node) {
				m_nodeMap[index] = node;
			}

			/// <summary>
			/// ノードの削除
			/// </summary>
			/// <param name="node">削除したいノード</param>
			virtual void RemoveNode(const std::shared_ptr<NodeType>& node) {
				//maru::MyUtility::RemoveVec(m_nodeMap, node);
			}

			/// <summary>
			/// エッジの追加
			/// </summary>
			/// <param name="edge">追加したいエッジ</param>
			virtual void AddEdge(const std::shared_ptr<EdgeType>& edge) {
				int index = edge->GetFromIndex();
				m_edgesMap[index].push_back(edge);
			}

			/// <summary>
			/// エッジの追加
			/// </summary>
			/// <param name="from">元のタイプ</param>
			/// <param name="to">遷移先のタイプ</param>
			/// <param name="isTransitionFunc">遷移条件</param>
			/// <param name="isEndTransition">終了時に遷移するかどうか</param>
			template<class... Ts,
				std::enable_if_t<
					std::is_constructible_v<EdgeType, std::shared_ptr<NodeBase>&, std::shared_ptr<NodeBase>&, Ts...>,	//コンストラクタの引数の整合性を保証する
				std::nullptr_t
			> = nullptr>
			void AddEdge(const int fromIndex, const int toIndex, Ts&&... params)
			{
				auto newEdge = std::make_shared<EdgeType>(GetNode(fromIndex), GetNode(toIndex), params...);
				AddEdge(newEdge);
			}

			/// <summary>
			/// ノードの数の取得
			/// </summary>
			/// <returns>ノードの数</returns>
			int GetNumNode() const noexcept {
				return static_cast<int>(m_nodeMap.size());
			}

			/// <summary>
			/// エッジの数の取得
			/// </summary>
			/// <param name="from">どのノードから伸びるエッジか指定</param>
			/// <returns>エッジの数</returns>
			virtual int GetNumEdge(const int from) const noexcept {
				return static_cast<int>(m_edgesMap.count(from));
			}

			/// <summary>
			/// エッジ連想配列の数を取得
			/// </summary>
			/// <returns>エッジの連想配列</returns>
			int GetNumEdgeMap() const noexcept {
				return static_cast<int>(m_edgesMap.size());
			}

			/// <summary>
			/// アクティブなグラフかどうか
			/// </summary>
			/// <returns>アクティブなグラフならtrue</returns>
			bool IsActive() const noexcept {
				return m_isActive;
			}


			/// <summary>
			/// グラフのアクティブ状態を設定
			/// </summary>
			/// <param name="isActive">グラフのアクティブ状態</param>
			void SetActive(const bool isActive) noexcept {
				m_isActive = isActive;
			}

			/// <summary>
			/// ノードの有無を判断
			/// </summary>
			/// <returns>ノードが一つも存在しないのならばtrue</returns>
			bool IsEmpty() const noexcept {
				return static_cast<int>(m_nodeMap.size()) == 0 ? true : false;
			}
		};

	}
}