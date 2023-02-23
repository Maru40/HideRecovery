/*!
@file Ticket_AstarRoute.h
@brief Ticket_AstarRoute
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class AstarThreadController;
	namespace maru {
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
	}

	namespace Ticket {

		//--------------------------------------------------------------------------------------
		///	前方宣言
		//--------------------------------------------------------------------------------------
		

		//--------------------------------------------------------------------------------------
		///	ルート発行用チケット
		//--------------------------------------------------------------------------------------
		class AstarRoute
		{
		public:
			friend class AstarThreadController;

			using RouteStack = std::stack<std::weak_ptr<maru::AstarNode>>;						//検索ルートのスタック
			using GraphType = maru::SparseGraph < maru::AstarNode, maru::AstarEdge, nullptr> ;	//グラフタイプ設定

		private:
			bool m_isValid;		//有効状態であるかどうか

			/// <summary>
			/// 有効状態の設定
			/// </summary>
			/// <param name="isValid"></param>
			void SetIsValid(const bool isValid) { m_isValid = isValid; }

			RouteStack m_route;	//検索したルート

		public:
			AstarRoute();

		private:

			/// <summary>
			/// ルート検索開始
			/// </summary>
			/// <param name="startNode">開始ノード</param>
			/// <param name="targetNode">ターゲットノード</param>
			/// <param name="graph">グラフ</param>
			void Start_RouteSearch(
				const std::shared_ptr<maru::AstarNode>& startNode, 
				const std::shared_ptr<maru::AstarNode>& targetNode,
				const std::shared_ptr<GraphType>& graph
			);

		public:

			//--------------------------------------------------------------------------------------
			///	アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 有効状態であるかどうか
			/// </summary>
			/// <returns>有効状態ならtrue</returns>
			bool IsValid() const { return m_isValid; }

			/// <summary>
			/// 検索したルートの取得
			/// </summary>
			/// <returns>ルート</returns>
			RouteStack GetRoute() const;

		};

	}
}