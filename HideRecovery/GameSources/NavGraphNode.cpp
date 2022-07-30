
/*!
@file NavGraphNode.cpp
@brief NavGraphNode�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "NavGraphNode.h"

namespace basecross {

	NavGraphNode::NavGraphNode()
		: NavGraphNode(0)
	{ }

	NavGraphNode::NavGraphNode(const int& index)
		: NavGraphNode(index, Vec3(0.0f))
	{ }

	NavGraphNode::NavGraphNode(const int& index, const Vec3& position)
		: NavGraphNode(index, position, ImpactData())
	{ }

	NavGraphNode::NavGraphNode(const int& index, const Vec3& position, const ImpactData& impactData)
		: NavGraphNode(index, position, impactData, nullptr)
	{}

	NavGraphNode::NavGraphNode(const int& index, const Vec3& position, const maru::ImpactData& impactData, const std::shared_ptr<GameObject>& parent)
		: GraphNodeBase(index), m_position(position), m_impactData(impactData), m_parent(parent) 
	{}
	
	Vec3 NavGraphNode::GetPosition() const {
		//�e�̐ݒ肪����Ȃ�A�e�̏ꏊ�̑��Έʒu��Ԃ��B
		if (IsParent()) {
			auto parentTrans = m_parent->GetComponent<Transform>();
			return m_position + parentTrans->GetWorldPosition();
		}

		return m_position;
	}

}

//endbasecross