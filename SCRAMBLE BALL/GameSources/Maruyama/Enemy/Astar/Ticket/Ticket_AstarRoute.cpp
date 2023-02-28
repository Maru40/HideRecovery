
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

#include "Maruyama/Enemy/Astar/OpenDataCalculater.h"

#include "Maruyama/Utility/Utility.h"
#include "Watanabe/DebugClass/Debug.h"

namespace basecross {
	namespace Ticket {

		AstarRoute::AstarRoute():
			m_isValid(false)
		{}

		void AstarRoute::Start_RouteSearch(
			const std::shared_ptr<maru::AstarNode>& startNode,
			const std::shared_ptr<maru::AstarNode>& targetNode,
			const std::shared_ptr<GraphType>& graph
		) {
			SetIsValid(false);								//検索開始したため、無効状態に変更(将来的には別な場所で変更)

			maru::Utility::StackClear(m_route);				//現在のルートのクリア
			
			auto calculater = maru::OpenDataCalculater();	//オープンデータ管理クラス
			auto isSuccess = calculater.StartSearchAstar(startNode, targetNode, graph);	//計算開始

			if (!isSuccess) {
				Debug::GetInstance()->Log(L"Ticket::AstarRoute::Start_RouteSearch() : ルート検索に失敗しました。");
			}

			m_route = calculater.GetRoute();	//ルートの取得
			SetIsValid(true);					//有効状態に変更
		}

		AstarRoute::RouteStack AstarRoute::GetRoute() const {
			return m_route;
		}

	}
}