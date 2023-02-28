/*!
@file AstarThreadController.h
@brief AstarThreadController
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class AstarRouteRequester;
	class ThreadPool;
	namespace maru {
		class AstarNode;
		class AstarEdge_Ex;

		template<class NodeType, class EdgeType,
			std::enable_if_t<
				std::is_base_of_v<NodeBase, NodeType>&&		//NodeTypeがNodeBaseを継承していることを保証する
				std::is_base_of_v<EdgeBase, EdgeType>,		//EdgeTypeがEdgeBaseを継承していることを保証する
			std::nullptr_t>
		>
		class SparseGraph;
	}

	namespace Ticket {
		class AstarRoute;
	}

	//--------------------------------------------------------------------------------------
	///	Astarのスレッドコントローラー
	//--------------------------------------------------------------------------------------
	class AstarThreadController
	{
	public:
		using GraphType = maru::SparseGraph<maru::AstarNode, maru::AstarEdge, nullptr>;

	private:
		std::unique_ptr<ThreadPool> m_threadPool;	//スレッドプール

		std::unordered_map<AstarRouteRequester*, std::shared_ptr<Ticket::AstarRoute>> m_ticketMap;	//チケットマップ

	public:
		AstarThreadController(const std::uint_fast32_t& threadCount = 1);

		virtual ~AstarThreadController() = default;

	private:
		/// <summary>
		/// チケットの生成
		/// </summary>
		/// <returns></returns>
		std::shared_ptr<Ticket::AstarRoute> CreateTicket();

		/// <summary>
		/// チケットの発行依頼
		/// </summary>
		/// <param name="requester">チケット発行依頼者</param>
		/// <returns>発行したチケット</returns>
		_NODISCARD std::shared_ptr<Ticket::AstarRoute> PublishTicket(const std::shared_ptr<AstarRouteRequester>& requester);

		/// <summary>
		/// チケットの登録
		/// </summary>
		/// <param name="requester">申請者</param>
		/// <param name="ticket">登録したいチケット</param>
		void RegisterTicket(
			const std::shared_ptr<AstarRouteRequester>& requester, 
			const std::shared_ptr<Ticket::AstarRoute>& ticket
		);

		/// <summary>
		/// チケット登録解除
		/// </summary>
		/// <param name="requester">登録者</param>
		void UnRegisterTicket(const std::shared_ptr<AstarRouteRequester>& requester);

		/// <summary>
		/// すでにチケットが登録されているかどうか
		/// </summary>
		/// <param name="requester">登録者</param>
		bool HasRegisterTicket(const std::shared_ptr<AstarRouteRequester>& requester) const;

		/// <summary>
		/// チケットの削除申請
		/// </summary>
		/// <param name="requester">削除申請者</param>
		/// <returns>削除が成功したかどうか</returns>
		void DeleteTicket(const std::shared_ptr<AstarRouteRequester>& requester);

		/// <summary>
		/// チケットが重複しているかどうか
		/// </summary>
		/// <returns>重複しているならtrue</returns>
		bool IsDuplicationTicket(const std::shared_ptr<AstarRouteRequester>& requester);

	public:
		/// <summary>
		/// ルートの検索開始
		/// </summary>
		/// <param name="requester">リクエスト者</param>
		/// <param name="startNode">開始ノード</param>
		/// <param name="targetNode">目標ノード</param>
		/// <param name="graph">グラフ</param>
		/// <returns>検索チケット</returns>
		std::shared_ptr<Ticket::AstarRoute> Start_RouteSearch(
			const std::shared_ptr<AstarRouteRequester>& requester,
			const std::shared_ptr<maru::AstarNode>& startNode,
			const std::shared_ptr<maru::AstarNode>& targetNode,
			const std::shared_ptr<GraphType>& graph
		);
		
	};

}