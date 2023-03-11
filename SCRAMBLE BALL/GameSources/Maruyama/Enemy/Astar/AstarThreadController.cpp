
/*!
@file AstarThreadController.cpp
@brief AstarThreadController�̃N���X����
�S���F�ێR�T��
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

#include "Maruyama/Enemy/Component/SelfAstarNodeController_Ex.h"

namespace basecross {

	AstarThreadController::AstarThreadController(const std::uint_fast32_t& threadCount) :
		m_threadPool(new ThreadPool(threadCount))
	{}

	std::shared_ptr<Ticket::AstarRoute> AstarThreadController::CreateTicket() {
		return std::make_shared<Ticket::AstarRoute>();	//�`�P�b�g�̔��s
	}

	std::shared_ptr<Ticket::AstarRoute> AstarThreadController::PublishTicket(const std::shared_ptr<AstarRouteRequester>& requester) {
		//�`�P�b�g�̏d���m�F
		if (IsDuplicationTicket(requester)) {
			DeleteTicket(requester);		//�폜�\��
		}

		auto ticket = CreateTicket();		//�`�P�b�g�̐���
		RegisterTicket(requester, ticket);	//�`�P�b�g�̓o�^

		return ticket;						//���s�����`�P�b�g��Ԃ�
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

	bool AstarThreadController::HasRegisterTicket(const std::shared_ptr<AstarRouteRequester>& requester) const {
		if (m_ticketMap.count(requester.get()) == 0) {
			return false;	//����Ȃ��Ȃ�A�o�^����Ă��Ȃ��B
		}

		return m_ticketMap.at(requester.get()) != nullptr;	//nullptr�Ȃ�o�^����Ă��Ȃ��B
	}

	void AstarThreadController::DeleteTicket(const std::shared_ptr<AstarRouteRequester>& requester) {
		m_threadPool->DeleteTask(requester.get());	//�X���b�h�ɓo�^�����`�P�b�g�̍폜
		UnRegisterTicket(requester);				//�o�^����
	}

	bool AstarThreadController::IsDuplicationTicket(const std::shared_ptr<AstarRouteRequester>& requester) {
		return m_ticketMap.count(requester.get()) != 0;	//�}�b�v�̃J�E���g��0�Ȃ�o�^�O�B
	}

	std::shared_ptr<Ticket::AstarRoute> AstarThreadController::Start_RouteSearch(
		const std::shared_ptr<AstarRouteRequester>& requester,
		const std::shared_ptr<maru::AstarNode>& startNode,
		const std::shared_ptr<maru::AstarNode>& targetNode,
		const std::shared_ptr<GraphType>& graph
	) {
		auto ticket = PublishTicket(requester);	//�`�P�b�g�̔��s

		m_threadPool->Submit(requester.get(), &Ticket::AstarRoute::Start_RouteSearch, ticket, startNode, targetNode, graph);	//�X���b�h�Ƀ^�X�N�̈˗�
		
		return ticket;
	}

	std::future<std::shared_ptr<maru::AstarNode>> AstarThreadController::Start_SelfAstarNodeSearch(
		const std::shared_ptr<I_SelfAstarNodeRequester>& requester,
		const std::shared_ptr<GraphType>& graph
	) {
		return m_threadPool->Submit(requester.get(), &I_SelfAstarNodeRequester::Search_SelfAstarNode, requester.get());
	}

}