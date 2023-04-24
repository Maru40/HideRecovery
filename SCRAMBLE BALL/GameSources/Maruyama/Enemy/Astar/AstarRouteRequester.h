/*!
@file AstarRouteRequester.h
@brief AstarRouteRequester
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"

#include "Maruyama/Thread/ThreadPool.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	namespace maru {
		class AstarNode;
	}

	namespace Ticket {
		class AstarRoute;
	}

	//--------------------------------------------------------------------------------------
	/// AstarRoute�̃��N�G�X�g��
	//--------------------------------------------------------------------------------------
	class AstarRouteRequester : public Component, public I_ThreadRequester
	{
		std::stack<std::weak_ptr<maru::AstarNode>> m_route;	//�v�Z�ŏo�������[�g

		std::weak_ptr<Ticket::AstarRoute> m_ticket;			//���s���̃`�P�b�g

	public:
		AstarRouteRequester(const std::shared_ptr<GameObject>& owner);

		virtual ~AstarRouteRequester() = default;

		void OnUpdate() override;

	private:
		//���[�g�̍X�V(�����)
		void RouteUpdate();

		//���[�g�����̃��N�G�X�g(�܂����̏��)
		void Request();

	public:

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		bool HasTicket() const { return !m_ticket.expired(); }
		
		std::shared_ptr<Ticket::AstarRoute> GetTicket() const;

	private:
		void SetTicket(const std::shared_ptr<Ticket::AstarRoute>& ticket);
	};

}