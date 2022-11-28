/*!
@file I_Edge.cpp
@brief I_Edge‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "I_Edge.h"

#include "I_Node.h"
#include "I_PriorityController.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			EdgeBase::EdgeBase(
				const std::shared_ptr<I_Node>& fromNode, 
				const std::shared_ptr<I_Node>& toNode
			) :
				m_fromNode(fromNode),
				m_toNode(toNode)
			{ }

			EdgeBase::EdgeBase(
				const std::shared_ptr<I_Node>& fromNode,
				const std::shared_ptr<I_Node>& toNode,
				const std::shared_ptr<I_PriorityController> priorityController
			) :
				m_fromNode(fromNode),
				m_toNode(toNode),
				m_priorityContorller(priorityController)
			{ }

			void EdgeBase::SetPriority(const float priority) { m_priorityContorller->SetPriority(priority); }

			float EdgeBase::GetPriority() const { 
				//if (!GetToNode()->CanTransition()) {	
				//	return FLT_MAX;	//‘JˆÚ‚Å‚«‚È‚¢‚Ì‚È‚çA—Dæ‡ˆÊ‚ðÅ’á’l‚Æ‚µ‚Ä•Ô‚·B
				//}

				return m_priorityContorller->GetPriority();
			}

			float EdgeBase::CalculatePriority() {
				return m_priorityContorller->CalculatePriority();
			}

			void EdgeBase::SetPriorityController(const std::shared_ptr<I_PriorityController>& priorityController) {
				m_priorityContorller = priorityController;
			}

			std::shared_ptr<I_PriorityController> EdgeBase::GetPriorityContorller() const {
				return m_priorityContorller;
			}

		}
	}
}