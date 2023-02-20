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
		> = nullptr>
		class SparseGraph;
	}

	namespace Ticket {

		//--------------------------------------------------------------------------------------
		///	前方宣言
		//--------------------------------------------------------------------------------------
		class FriendTest;

		//--------------------------------------------------------------------------------------
		///	ルート発行用チケット
		//--------------------------------------------------------------------------------------
		class AstarRoute
		{
		public:
			//friend FriendTest;	//テストフレンド指定(将来的に消去)

			using GraphType = maru::SparseGraph<maru::AstarNode, maru::AstarEdge>;	//グラフタイプ設定

		private:
			bool m_isValid;		//有効状態であるかどうか

			/// <summary>
			/// 有効状態の設定
			/// </summary>
			/// <param name="isValid"></param>
			void SetIsValid(const bool isValid) { m_isValid = isValid; }

			std::queue<std::weak_ptr<maru::AstarNode>> m_route;	//検索したルート

		public:


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

		};
		
		class FriendTest
		{
			void Test(AstarRoute route) {
				
			}
		};

	}
}