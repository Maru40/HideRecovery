/*!
@file AstarRouteRequester.h
@brief AstarRouteRequester
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"

#include "Maruyama/Thread/ThreadPool.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	namespace maru {
		class AstarNode;
	}

	namespace Ticket {
		class AstarRoute;
	}

	//--------------------------------------------------------------------------------------
	/// AstarRouteのリクエスト者
	//--------------------------------------------------------------------------------------
	class AstarRouteRequester : public Component, public I_ThreadRequester
	{
		std::stack<std::weak_ptr<maru::AstarNode>> m_route;	//計算で出したルート

		std::weak_ptr<Ticket::AstarRoute> m_ticket;			//発行中のチケット

	public:
		AstarRouteRequester(const std::shared_ptr<GameObject>& owner);

		virtual ~AstarRouteRequester() = default;

		void OnUpdate() override;

	private:
		//ルートの更新(仮状態)
		void RouteUpdate();

		//ルート検索のリクエスト(まだ仮の状態)
		void Request();

	public:

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		bool HasTicket() const { return !m_ticket.expired(); }
		
		std::shared_ptr<Ticket::AstarRoute> GetTicket() const;

	private:
		void SetTicket(const std::shared_ptr<Ticket::AstarRoute>& ticket);
	};

}