
/*!
@file AstarNode.cpp
@brief AstarNode�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "AstarNode.h"

namespace basecross {

	namespace maru {

		AstarNode::AstarNode():
			AstarNode(0)
		{}

		AstarNode::AstarNode(const int index) :
			AstarNode(index, Vec3(0.0f))
		{}

		AstarNode::AstarNode(const int index, const Vec3& position) :
			AstarNode(index, position, nullptr)
		{}

		AstarNode::AstarNode(const int index, const Vec3& position, const std::shared_ptr<AstarNode>& parent):
			NodeBase(index), 
			m_position(position), 
			m_parent(parent)
		{}

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		void AstarNode::SetParent(const std::shared_ptr<AstarNode>& parent) noexcept {
			m_parent = parent;
		}

		std::shared_ptr<AstarNode> AstarNode::GetParent() const noexcept {
			return m_parent.lock();
		}

		bool AstarNode::HasParent() const noexcept {
			return !m_parent.expired();
		}
	}

}