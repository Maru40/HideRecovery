
/*!
@file AstarRouteRequester.cpp
@brief AstarRouteRequesterのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "AstarRouteRequester.h"

#include "Maruyama/Enemy/AIDirector/AIDirector_Ex.h"

#include "Maruyama/Enemy/Astar/AstarNode.h"
#include "Maruyama/Enemy/Astar/AstarEdge_Ex.h"
#include "Maruyama/Patch/Graphs/SparseGraph_Ex.h"

#include "Maruyama/Enemy/Astar/Ticket/Ticket_AstarRoute.h"
#include "Maruyama/Enemy/Astar/AstarThreadController.h"

#include "Maruyama/Enemy/Component/SelfAstarNodeController.h"


namespace basecross {

	AstarRouteRequester::AstarRouteRequester(const std::shared_ptr<GameObject>& owner):
		Component(owner)
	{}

	void AstarRouteRequester::OnUpdate() {
		if (!HasTicket()) {	//チケットをもっていなかったら
			return;
		}

		//チケットが有効状態なら、
		if (GetTicket()->IsValid()) {
			RouteUpdate();	//ルートを更新
		}
	}

	void AstarRouteRequester::RouteUpdate() {
		m_route = GetTicket()->GetRoute();	//計算したルートの取得

		m_ticket.reset();	//チケットの破棄
	}
	
	void AstarRouteRequester::Request() {
		using namespace Enemy;
		using namespace maru;

		auto director = AIDirector_Ex::GetInstance();
		auto& astarThread = director->GetAstarThreadController();

		//テスト
		auto testNode = std::make_shared<maru::AstarNode>();
		auto testGraph = std::make_shared <maru::SparseGraph<AstarNode, AstarEdge, nullptr>>();

		auto ticket = astarThread->Start_RouteSearch(GetThis<AstarRouteRequester>(), testNode, testNode, testGraph);
		SetTicket(ticket);	//チケットの更新
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	std::shared_ptr<Ticket::AstarRoute> AstarRouteRequester::GetTicket() const { return m_ticket.lock(); }

	void AstarRouteRequester::SetTicket(const std::shared_ptr<Ticket::AstarRoute>& ticket) { m_ticket = ticket; }

}