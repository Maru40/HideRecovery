
/*!
@file EdgeBase.cpp
@brief EdgeBase�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "EdgeBase.h"

#include "NodeBase.h"

namespace basecross {

	namespace maru {

		EdgeBase::EdgeBase():
			EdgeBase(nullptr, nullptr)
		{}

		EdgeBase::EdgeBase(const std::shared_ptr<NodeBase>& fromNode, const std::shared_ptr<NodeBase>& toNode) :
			m_fromNode(fromNode),
			m_toNode(toNode)
		{}

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		//--------------------------------------------------------------------------------------
		/// FromNode
		//--------------------------------------------------------------------------------------

		void EdgeBase::SetFromNode(const std::shared_ptr<NodeBase>& node) {
			m_fromNode = node;
		}

		std::shared_ptr<NodeBase> EdgeBase::GetFromNode() const {
			return m_fromNode.lock();
		}

		void EdgeBase::SetFromIndex(const int index) noexcept {
			GetFromNode()->SetIndex(index);
		}

		int EdgeBase::GetFromIndex() const noexcept {
			return GetFromNode()->GetIndex();
		}

		//--------------------------------------------------------------------------------------
		/// ToNode
		//--------------------------------------------------------------------------------------

		void EdgeBase::SetToNode(const std::shared_ptr<NodeBase>& node) {
			m_toNode = node;
		}

		std::shared_ptr<NodeBase> EdgeBase::GetToNode() const {
			return m_toNode.lock();
		}

		void EdgeBase::SetToIndex(const int index) noexcept {
			GetToNode()->SetIndex(index);
		}

		int EdgeBase::GetToIndex() const noexcept {
			return GetToNode()->GetIndex();
		}

		//--------------------------------------------------------------------------------------
		/// ���̑��A�N�Z�b�T
		//--------------------------------------------------------------------------------------


	}
}