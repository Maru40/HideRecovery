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
				const std::shared_ptr<I_Node>& toNode,
				const float priority
			) :
				m_fromNode(fromNode),
				m_toNode(toNode),
				m_priority(priority)
			{ }

			float EdgeBase::CalculatePriority() {
				//—Dæ“x‚ÌŒvŽZ
				for (auto& priorityController : m_priorityControllers) {
					priorityController->CalculatePriority();
				}

				return GetPriority();
			}

			void EdgeBase::AddPriorityController(const std::shared_ptr<I_PriorityController>& priorityController) {
				m_priorityControllers.push_back(priorityController);
			}
		}
	}
}