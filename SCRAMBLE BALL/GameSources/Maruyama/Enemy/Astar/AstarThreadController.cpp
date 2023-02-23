
/*!
@file AstarThreadController.cpp
@brief AstarThreadControllerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Astar/AstarNode.h"
#include "Maruyama/Enemy/Astar/AstarEdge_Ex.h"
#include "AstarThreadController.h"

#include "Maruyama/Thread/ThreadPool.h"

#include "Maruyama/Enemy/Astar/AstarNode.h"
#include "Maruyama/Enemy/Astar/AstarEdge_Ex.h"
#include "Ticket/Ticket_AstarRoute.h"
#include "AstarRouteRequester.h"

namespace basecross {

	AstarThreadController::AstarThreadController(const std::uint_fast32_t& threadCount) :
		m_threadPool(new ThreadPool(threadCount))
	{}

	std::shared_ptr<Ticket::AstarRoute> AstarThreadController::CreateTicket() {
		return std::make_shared<Ticket::AstarRoute>();	//チケットの発行
	}

	std::shared_ptr<Ticket::AstarRoute> AstarThreadController::PublishTicket(const std::shared_ptr<AstarRouteRequester>& requester) {
		//チケットの重複確認
		if (IsDuplicationTicket(requester)) {
			DeleteTicket(requester);		//削除申請
		}

		auto ticket = CreateTicket();		//チケットの生成
		RegisterTicket(requester, ticket);	//チケットの登録

		return ticket;						//発行したチケットを返す
	}

	void AstarThreadController::RegisterTicket(
		const std::shared_ptr<AstarRouteRequester>& requester,
		const std::shared_ptr<Ticket::AstarRoute>& ticket
	) {
		m_ticketMap[requester.get()] = ticket;
	}

	void AstarThreadController::UnRegisterTicket(const std::shared_ptr<AstarRouteRequester>& requester) {
		m_ticketMap[requester.get()] = nullptr;
	}

	void AstarThreadController::DeleteTicket(const std::shared_ptr<AstarRouteRequester>& requester) {
		m_threadPool->DeleteTask(requester.get());	//スレッドに登録したチケットの削除
		UnRegisterTicket(requester);				//登録解除
	}

	bool AstarThreadController::IsDuplicationTicket(const std::shared_ptr<AstarRouteRequester>& requester) {
		return m_ticketMap.count(requester.get()) != 0;	//マップのカウントが0なら登録前。
	}

	std::shared_ptr<Ticket::AstarRoute> AstarThreadController::Start_RouteSearch(
		const std::shared_ptr<AstarRouteRequester>& requester,
		const std::shared_ptr<maru::AstarNode>& startNode,
		const std::shared_ptr<maru::AstarNode>& targetNode,
		const std::shared_ptr<GraphType>& graph
	) {
		auto ticket = PublishTicket(requester);	//チケットの発行

		m_threadPool->Submit(requester.get(), &Ticket::AstarRoute::Start_RouteSearch, ticket, startNode, targetNode, graph);	//スレッドにタスクの依頼
		
		return ticket;
	}

}