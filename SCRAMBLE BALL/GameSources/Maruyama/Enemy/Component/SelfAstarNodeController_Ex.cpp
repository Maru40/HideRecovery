
/*!
@file SelfAstarNodeController_Ex.cpp
@brief SelfAstarNodeController_Ex�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "SelfAstarNodeController_Ex.h"

namespace basecross {
	namespace maru {

		SelfAstarNodeController_Ex::SelfAstarNodeController_Ex(const std::shared_ptr<GameObject>& owner):
			Component(owner)
		{}

		void SelfAstarNodeController_Ex::OnUpdate() {
			//m_future.
		}

		std::shared_ptr<AstarNode> SelfAstarNodeController_Ex::Search_SelfAstarNode() {
			return nullptr;
		}

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		std::shared_ptr<AstarNode> SelfAstarNodeController_Ex::GetCurrentNode() const { return m_currentNode.lock(); }

		void SelfAstarNodeController_Ex::SetCurrentNode(const std::shared_ptr<AstarNode>& node) { m_currentNode = node; }

	}
}