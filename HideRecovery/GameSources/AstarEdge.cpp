
/*!
@file AstarEdge.cpp
@brief AstarEdge�̃N���X����
�S���F�ێR�T��
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
		//astar�̐�̃m�[�h
		const auto graph = astar->GetGraph();
		auto fromNode = graph->GetNode(m_from);
		auto toNode = graph->GetNode(m_to);

		//�ǂ���̃m�[�h���A�N�e�B�u��ԂȂ�true
		return fromNode->IsActive() && toNode->IsActive();
	}

}

//endbasecross