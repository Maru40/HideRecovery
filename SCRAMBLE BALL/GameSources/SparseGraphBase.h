
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
			NodeMap m_nodeMap;			//ノードを格納する配列
			EdgeVectorMap m_edgesMap;	//エッジを格納する配列

			bool m_isActive = true;		//アクティブかどうか

		public:
			SparseGraphBase() = default;
			virtual ~SparseGraphBase() = default;

		};

	}
}