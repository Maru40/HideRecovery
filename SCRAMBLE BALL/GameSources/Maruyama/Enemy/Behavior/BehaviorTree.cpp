/*!
@file BehaviorTree.cpp
@brief BehaviorTreeなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "BehaviorTree.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			//--------------------------------------------------------------------------------------
			/// ビヘイビアタスクの基底クラス
			//--------------------------------------------------------------------------------------

			bool NodeBase::CanTransition() const {
				if (IsDecoratorEmpty()) {	//デコレータがないなら、常にtrue
					return true;
				}

				//一つでも遷移できないならfalse
				for (const auto& decorator : m_decorators) {
					if (!decorator->CanTransition()) {
						return false;
					}
				}

				return true;	//全てのデコレータがtrueなら遷移できる。
			}

			void NodeBase::AddDecorator(const std::shared_ptr<I_Decorator>& decorator) {
				m_decorators.push_back(decorator);
			}

			bool NodeBase::IsDecoratorEmpty() const {
				return static_cast<int>(m_decorators.size()) == 0;
			}

			//--------------------------------------------------------------------------------------
			/// ビヘイビアセレクターの基底クラス
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 遷移先ノードを優先順位順にソートする条件
			/// </summary>
			/// <param name="leftData"></param>
			/// <param name="rightData"></param>
			/// <returns></returns>
			bool SortTransitionNodeData(
				const std::shared_ptr<I_Selecter::TransitionNodeData> leftData, 
				const std::shared_ptr<I_Selecter::TransitionNodeData> rightData
			) {
				//return true;
				return leftData->priorityController->GetPriority() < rightData->priorityController->GetPriority();
			}

			SelecterBase::SelecterBase(const std::shared_ptr<I_Node>& fromNode) :
				m_fromNode(fromNode)
			{}

			std::shared_ptr<I_Node> SelecterBase::GetFirstPriorityNode() const {
				if (IsEmptyTransitionNodes()) {	//遷移先ノードが空ならnullptr
					return nullptr;
				}

				for (auto data : m_transitionNodes) {
					data;
					data->priorityController;
					auto isMax = data->priorityController->GetPriority() < data->priorityController->GetPriority();

					SortTransitionNodeData(data, data);
				}

				//std::sort(m_transitionNodes.begin(), m_transitionNodes.end(), &SortTransitionNodeData);

				//return nullptr;
				return m_transitionNodes[0]->node.lock();	//一番優先度が高いノードを返す。
			}

			bool SelecterBase::IsEmptyTransitionNodes() const {
				return static_cast<int>(m_transitionNodes.size()) == 0;
			}

		}

	}
}