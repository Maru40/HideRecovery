
/*!
@file AstarEdge.cpp
@brief AstarEdgeのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Astar/AstarNode.h"
#include "Maruyama/Enemy/Astar/AstarEdge_Ex.h"
#include "Ticket_AstarRoute.h"

#include "Maruyama/Enemy/Astar/OpenDataHandler.h"

#include "Maruyama/Utility/Utility.h"

namespace basecross {
	namespace Ticket {

		void AstarRoute::Start_RouteSearch(
			const std::shared_ptr<maru::AstarNode>& startNode,
			const std::shared_ptr<maru::AstarNode>& targetNode,
			const std::shared_ptr<GraphType>& graph
		) {
			SetIsValid(false);	//検索開始したため、無効状態に変更(将来的には別な場所で変更)

			maru::Utility::QueueClear(m_route);	//現在のルートのクリア
			
			auto openHandler = OpenDataHandler();	//オープンデータ管理クラス
			//openHandler.StartSearchAstar(startNode, targetNode, graph);
		}

	}
}