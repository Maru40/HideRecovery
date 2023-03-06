
/*!
@file AstarRouteRequester.cpp
@brief AstarRouteRequester�̃N���X����
�S���F�ێR�T��
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
		if (!HasTicket()) {	//�`�P�b�g�������Ă��Ȃ�������
			return;
		}

		//�`�P�b�g���L����ԂȂ�A
		if (GetTicket()->IsValid()) {
			RouteUpdate();	//���[�g���X�V
		}
	}

	void AstarRouteRequester::RouteUpdate() {
		m_route = GetTicket()->GetRoute();	//�v�Z�������[�g�̎擾

		m_ticket.reset();	//�`�P�b�g�̔j��
	}
	
	void AstarRouteRequester::Request() {
		using namespace Enemy;
		using namespace maru;

		auto director = AIDirector_Ex::GetInstance();
		auto& astarThread = director->GetAstarThreadController();

		//�e�X�g
		auto testNode = std::make_shared<maru::AstarNode>();
		auto testGraph = std::make_shared <maru::SparseGraph<AstarNode, AstarEdge, nullptr>>();

		auto ticket = astarThread->Start_RouteSearch(GetThis<AstarRouteRequester>(), testNode, testNode, testGraph);
		SetTicket(ticket);	//�`�P�b�g�̍X�V
	}

	//--------------------------------------------------------------------------------------
	/// �A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	std::shared_ptr<Ticket::AstarRoute> AstarRouteRequester::GetTicket() const { return m_ticket.lock(); }

	void AstarRouteRequester::SetTicket(const std::shared_ptr<Ticket::AstarRoute>& ticket) { m_ticket = ticket; }

}