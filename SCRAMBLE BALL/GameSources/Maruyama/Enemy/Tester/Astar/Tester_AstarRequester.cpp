/*!
@file Tester_AstarRequester.cpp
@brief Tester_AstarRequester�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "Tester_AstarRequester.h"

#include "Maruyama/Enemy/AIDirector/AIDirector_Ex.h"

#include "Maruyama/Enemy/Astar/AstarNode.h"
#include "Maruyama/Enemy/Astar/AstarEdge_Ex.h"
#include "Maruyama/Enemy/Astar/AstarThreadController.h"

#include "Maruyama/Patch/Graphs/SparseGraph_Ex.h"

#include "Maruyama/Enemy/Astar/Ticket/Ticket_AstarRoute.h"

namespace basecross {
	namespace Enemy {
		namespace Tester {

			void AstarRequester::Request() {
				using namespace maru;

				auto director = AIDirector_Ex::GetInstance();
				auto& threadController = director->GetAstarThreadController();

				//�e�X�g
				auto testNode = std::make_shared<maru::AstarNode>();
				auto testGraph = std::make_shared <maru::SparseGraph<AstarNode, AstarEdge, nullptr>>();

				//auto ticket =  threadController->Start_RouteSearch(GetThis<AstarRouteRequester>(), testNode, testNode, testGraph);

				//ticket->IsValid();
			}

		}
	}
}