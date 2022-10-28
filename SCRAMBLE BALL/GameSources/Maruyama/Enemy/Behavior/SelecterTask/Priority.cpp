/*!
@file I_Selecter.cpp
@brief I_Selecterなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "Priority.h"

#include "../Interface/I_Node.h"
#include "../Interface/I_Selecter.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace SelecterTask {

				Priority::Priority(const std::shared_ptr<Selecter>& selecter):
					TaskNodeBase(selecter)
				{}

				void Priority::OnStart() {
					auto selecter = GetOwner();

					//一番優先度の高いノードの取得
					//auto node = selecter->SearchFirstPriorityNode();
					//
					//selecter->ChangeCurrentNode(node);	//セレクターのカレントノードに設定
					//m_currentNode = node;				//自分自身のカレントノードに設定
				}

				bool Priority::OnUpdate() {
					if (HasCurrentNode()) {
						return m_currentNode.lock()->OnUpdate();
					}

					return true;
				}

				void Priority::OnExit() {
					m_currentNode.reset();
				}

				bool Priority::HasCurrentNode() const {
					return !m_currentNode.expired();
				}

			}
		}
	}
}