/*!
@file Tester_AstarRequester.h
@brief Tester_AstarRequester�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Enemy/Astar/AstarRouteRequester.h"

namespace basecross {
	namespace Ticket {
		class AstarRoute;
	}

	namespace Enemy {
		namespace Tester {

			class AstarRequester : public AstarRouteRequester {
			private:
				std::weak_ptr<Ticket::AstarRoute> m_ticket;

			public:
				virtual ~AstarRequester() = default;

			private:
				//Astar�̃��N�G�X�g
				void Request();

			};

		}
	}
}