
/*!
@file AstarEdge.cpp
@brief AstarEdgeのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "AstarEdge.h"

#include "Heuristic.h"
#include "SparseGraph.h"
#include "NavGraphNode.h"
#include "UtilityAstar.h"
#include "GraphAstar.h"

namespace basecross {

	AstarEdge::AstarEdge(const int& from, const int& to)
		: AstarEdge(from, to, 0.0f)
	{ }
	AstarEdge::AstarEdge(const int& from, const int& to, const float& cost)
		: m_from(from), m_to(to), m_cost(cost), m_isActive(true)
	{}

	bool AstarEdge::IsNodeActive(const std::shared_ptr<GraphAstar>& astar) const {
		//astarの先のノード
		const auto graph = astar->GetGraph();
		auto fromNode = graph->GetNode(m_from);
		auto toNode = graph->GetNode(m_to);

		//どちらのノードもアクティブ状態ならtrue
		return fromNode->IsActive() && toNode->IsActive();
	}

}

//endbasecross