/*!
@file Heuristic.cpp
@brief Heuristic‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "Heuristic.h"

namespace basecross {

	Heuristic::Heuristic() :
		Heuristic(nullptr)
	{}

	Heuristic::Heuristic(const std::shared_ptr<NavGraphNode>& node) :
		m_targetNode(node)
	{}

	float Heuristic::CalculateHeuristicRange(const std::shared_ptr<const NavGraphNode>& node) {
		auto position = node->GetPosition();
		auto targetPosition = m_targetNode->GetPosition();

		auto toVec = targetPosition - position;
		return toVec.length();
	}

}

//endbasecross