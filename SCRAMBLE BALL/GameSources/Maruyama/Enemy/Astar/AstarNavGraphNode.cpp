
/*!
@file NavGraphNode.cpp
@brief NavGraphNodeのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "AstarNavGraphNode.h"

#include "Maruyama/Utility/MaruAction.h"
#include "Maruyama/Utility/ReactiveProperty.h"

namespace basecross {

	AstarNavGraphNode::AstarNavGraphNode()
		: AstarNavGraphNode(0)
	{ }

	AstarNavGraphNode::AstarNavGraphNode(const int& index)
		: AstarNavGraphNode(index, Vec3(0.0f))
	{ }

	AstarNavGraphNode::AstarNavGraphNode(const int& index, const Vec3& position)
		: AstarNavGraphNode(index, position, ImpactData())
	{ }

	AstarNavGraphNode::AstarNavGraphNode(const int& index, const Vec3& position, const ImpactData& impactData)
		: AstarNavGraphNode(index, position, impactData, nullptr)
	{}

	AstarNavGraphNode::AstarNavGraphNode(const int& index, const Vec3& position, const maru::ImpactData& impactData, const std::shared_ptr<GameObject>& parent)
		: NavGraphNode(index, position, impactData, parent)
	{}

	//コピーコンストラクタ
	AstarNavGraphNode::AstarNavGraphNode(const AstarNavGraphNode& node) :
		AstarNavGraphNode(node.GetIndex(), node.GetPosition(), node.GetImpactData(), node.GetParent())
	{}

	//--------------------------------------------------------------------------------------
	///	アクセッサ
	//--------------------------------------------------------------------------------------

	void AstarNavGraphNode::SetGraphAstar(const std::shared_ptr<GraphAstar>& astar) noexcept {
		m_astar = astar;
	}

	std::shared_ptr<GraphAstar> AstarNavGraphNode::GetGraphAstar() const noexcept {
		return m_astar;
	}
}