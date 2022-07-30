
/*!
@file GraphBase.h
@brief GraphBase
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	template<class node_type>
	class StateNodeBase;

	class I_StateNode;

	template<class EnumType, class TransitionStructMember>
	class StateEdgeBase;

	class GraphStateNode;

	#define DECLARE_NODE(NodeName, NodeType, Enter, Update, Exit) class NodeName : public StateNodeBase<NodeType>{ public: NodeName(); void OnStart() override{ if(Enter){ Enter(); } }; void OnUpdate() override { if(Update){ Update(); } }; void OnExit(){ if(Exit){ Exit(); } }; };

	/// <summary>
	/// Graphの基底クラス
	/// </summary>
	/// <typeparam name="NodeType">使用するノード</typeparam>
	/// <typeparam name="EnumType">使用する列挙体</typeparam>
	/// <typeparam name="TransitionStructMember">遷移条件用の構造体メンバー</typeparam>
	template<class node_type, class EnumType, class TransitionStructMember>
	class GraphBase
	{
	public:
		//usingディレクティブ
		using EdgeType = StateEdgeBase<EnumType, TransitionStructMember>;

		using NodeType = node_type;
		using NodeMap = std::map<EnumType, std::shared_ptr<NodeType>>;
		using EdgeVector = std::vector<std::shared_ptr<EdgeType>>;
		using EdgeVectorMap = std::map<EnumType, EdgeVector>;

	private:
		NodeMap m_nodes;          //ノードを格納する配列
		EdgeVectorMap m_edgesMap; //エッジを格納する配列

		bool m_isActive = true;   //アクティブかどうか

	public:
		GraphBase() {}
		~GraphBase() {}

		/// <summary>
		/// 指定したノードの取得
		/// </summary>
		/// <param name="type">ノードタイプ</param>
		/// <returns>指定したノード</returns>
		std::shared_ptr<NodeType> GetNode(const EnumType type) const {
			return m_nodes.at(type);
		}

		/// <summary>
		/// ノードの配列を取得
		/// </summary>
		/// <returns>ノード配列</returns>
		NodeMap GetNodes() const {
			return m_nodes;
		}

		/// <summary>
		/// 指定したノードを連結するエッジの取得
		/// </summary>
		/// <param name="from">手前のインデックス</param>
		/// <param name="to">先のインデックス</param>
		/// <returns>指定したノードを連結するエッジの取得</returns>
		std::shared_ptr<EdgeType> GetEdge(const EnumType from, const EnumType to) const {
			//存在しなかったらnullptrを返す。
			if (m_edgesMap.count[from] == 0) {
				return nullptr;
			}

			auto edges = m_edgesMap[from];
			for (auto& edge : edges) {
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
		EdgeVector GetEdges(const EnumType from) const {
			if (m_edgesMap.count(from) == 0) {
				return EdgeVector();
			}

			return m_edgesMap.at(from);
		}

		/// <summary>
		/// エッジ連想配列を取得
		/// </summary>
		/// <returns>エッジ連想配列</returns>
		EdgeVectorMap GetEdgesMap() {
			return m_edgesMap;
		}

		/// <summary>
		/// ノードの追加
		/// </summary>
		/// <param name="type">ノードのタイプ</param>
		/// <param name="node">ノードのポインタ</param>
		virtual void AddNode(const EnumType type, const std::shared_ptr<NodeType>& node) {
			m_nodes[type] = node;
		}

		/// <summary>
		/// ノードの削除
		/// </summary>
		/// <param name="node">削除したいノード</param>
		void RemoveNode(const std::shared_ptr<NodeType>& node) {
			//maru::MyUtility::RemoveVec(m_nodes, node);
		}

		/// <summary>
		/// エッジの追加
		/// </summary>
		/// <param name="edge">追加したいエッジ</param>
		void AddEdge(const std::shared_ptr<EdgeType>& edge) {
			m_edgesMap[edge->GetFromType()].push_back(edge);
		}

		/// <summary>
		/// エッジの追加
		/// </summary>
		/// <param name="from">元のタイプ</param>
		/// <param name="to">遷移先のタイプ</param>
		/// <param name="isTransitionFunc">遷移条件</param>
		/// <param name="isEndTransition">終了時に遷移するかどうか</param>
		void AddEdge(const EnumType from, const EnumType to,
			const std::function<bool(const TransitionStructMember& transition)>& isTransitionFunc,
			const bool isEndTransition = false)
		{
			auto newEdge = std::make_shared<EdgeType>(from, to, isTransitionFunc, static_cast<int>(to),isEndTransition);
			AddEdge(newEdge);
		}

		/// <summary>
		/// ノードの数の取得
		/// </summary>
		/// <returns>ノードの数</returns>
		int GetNumNode() const {
			return static_cast<int>(m_nodes.size());
		}

		/// <summary>
		/// エッジの数の取得
		/// </summary>
		/// <param name="from">どのノードから伸びるエッジか指定</param>
		/// <returns>エッジの数</returns>
		int GetNumEdge(const EnumType from) const {
			return static_cast<int>(m_edgesMap.count(from));
		}

		/// <summary>
		/// エッジ連想配列の数を取得
		/// </summary>
		/// <returns>エッジの連想配列</returns>
		int GetNumEdgeMap() const {
			return static_cast<int>(m_edgesMap.size());
		}

		/// <summary>
		/// アクティブなグラフかどうか
		/// </summary>
		/// <returns>アクティブなグラフならtrue</returns>
		bool IsActive() {
			return m_isActive;
		}

		/// <summary>
		/// グラフのアクティブ状態を設定
		/// </summary>
		/// <param name="isActive">グラフのアクティブ状態</param>
		void SetActive(const bool isActive) {
			m_isActive = isActive;
		}

		/// <summary>
		/// ノードの有無を判断
		/// </summary>
		/// <returns>ノードが一つも存在しないのならばtrue</returns>
		bool IsEmpty() const {
			return static_cast<int>(m_nodes.size()) == 0 ? true : false;
		}
	};

}

//endbasecross


