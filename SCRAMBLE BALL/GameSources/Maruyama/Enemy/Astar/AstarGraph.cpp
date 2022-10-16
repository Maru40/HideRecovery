/*!
@file GraphAstar.cpp
@brief GraphAstar‚ÌƒNƒ‰ƒXŽÀ‘Ì
’S“–FŠÛŽR—TŠì
*/

#include "stdafx.h"
#include "Project.h"

#include "AstarGraph.h"

#include "Heuristic.h"
#include "SparseGraph.h"
#include "NavGraphNode.h"
#include "Maruyama/Enemy/Astar/AstarEdge.h"
#include "Maruyama/Enemy/Astar/UtilityAstar.h"

#include "Maruyama/Debug/Object/DebugObject.h"

namespace basecross {

	AstarGraph::AstarGraph(bool digraph) :
		SparseGraph(digraph)
	{}

	Vec3 AstarGraph::CalculateCenterPosition() {
		auto sumPosition = Vec3(0.0f);
		auto nodes = GetNodes();

		for (const auto& node : nodes) {
			sumPosition += node->GetPosition();
		}

		m_centerPosition = sumPosition / static_cast<float>(nodes.size());
		return m_centerPosition;
	}
}