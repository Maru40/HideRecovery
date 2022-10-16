/*!
@file I_Selecter.cpp
@brief I_Selecterなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "I_Selecter.h"

#include "I_PriorityController.h"

#include "Maruyama/Enemy/StateMachine/EnemyMainStateMachine.h"

namespace basecross {
	namespace maru {

		namespace Behavior {


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
			/// ソート用の完全隠蔽関数
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

			//--------------------------------------------------------------------------------------
			/// ビヘイビアセレクターの基底クラス
			//--------------------------------------------------------------------------------------

			SelecterBase::SelecterBase() :
				SelecterBase(SelectType::Priority)
			{}

			SelecterBase::SelecterBase(const SelectType selectType) :
				m_selectType(selectType)
			{}

			void SelecterBase::OnStart() {

			}

			bool SelecterBase::OnUpdate() {
				return true;
			}

			void SelecterBase::OnExit() {

			}
				
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

			std::shared_ptr<I_Node> SelecterBase::GetCurrentNode() const {
				auto currentTransitionData = m_currentTransitionData.lock();
				if (!currentTransitionData) {
					return nullptr;
				}

				return currentTransitionData->node.lock();
			}

			

		}
	}
}