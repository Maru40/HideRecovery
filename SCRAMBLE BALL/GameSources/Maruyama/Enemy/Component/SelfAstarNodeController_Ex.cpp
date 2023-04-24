
/*!
@file SelfAstarNodeController_Ex.cpp
@brief SelfAstarNodeController_Exのクラス実体
担当：丸山裕喜
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
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		std::shared_ptr<AstarNode> SelfAstarNodeController_Ex::GetCurrentNode() const { return m_currentNode.lock(); }

		void SelfAstarNodeController_Ex::SetCurrentNode(const std::shared_ptr<AstarNode>& node) { m_currentNode = node; }

	}
}