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
			/// ビヘイビアセレクターの遷移先ノードデータ
			//--------------------------------------------------------------------------------------

			Selecter_TransitionNodeData::Selecter_TransitionNodeData(
				const std::shared_ptr<I_PriorityController>& priorityController,
				const std::shared_ptr<I_Node>& node
			) :
				priorityController(priorityController),
				node(node)
			{}

			//--------------------------------------------------------------------------------------
			/// ビヘイビアセレクターの基底クラス
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 遷移先ノードを優先順位順にソートする条件
			/// </summary>
			/// <param name="leftData">比較対象のデータ</param>
			/// <param name="rightData">比較対象のデータ</param>
			/// <returns>leftDataが小さいならtrue</returns>
			bool SortTransitionNodeData(
				const std::shared_ptr<I_Selecter::TransitionNodeData> leftData, 
				const std::shared_ptr<I_Selecter::TransitionNodeData> rightData
			) {
				return leftData->priorityController->GetPriority() < rightData->priorityController->GetPriority();
			}

			SelecterBase::SelecterBase(const std::shared_ptr<I_Node>& fromNode) :
				m_fromNode(fromNode)
			{}

			std::shared_ptr<I_Node> SelecterBase::GetFirstPriorityNode() const {
				if (IsEmptyTransitionNodes()) {	//遷移先ノードが空ならnullptr
					return nullptr;
				}

				auto transitionDatas = m_transitionDatas;	//メンバをソートするとconstにできないため、一時変数化
				std::sort(transitionDatas.begin(), transitionDatas.end(), &SortTransitionNodeData);	//昇順ソート

				return transitionDatas[0]->node.lock();	//一番優先度が高いノードを返す。
			}

			bool SelecterBase::IsEmptyTransitionNodes() const {
				return static_cast<int>(m_transitionDatas.size()) == 0;
			}

		}

	}
}