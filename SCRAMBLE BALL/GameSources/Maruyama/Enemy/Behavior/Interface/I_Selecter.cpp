/*!
@file I_Selecter.cpp
@brief I_Selecterなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "I_Selecter.h"

#include "I_Edge.h"
#include "I_PriorityController.h"

#include "Maruyama/TaskList/TaskList.h"

#include "Maruyama/Utility/Random.h"

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

			/// <summary>
			/// 遷移先を優先順位順にソートする条件
			/// </summary>
			/// <param name="right">比較対象のデータ</param>
			/// <param name="left">比較対象のデータ</param>
			/// <returns>leftが小さいならtrue</returns>
			bool SortEdges(
				const std::weak_ptr<I_Edge>& left,
				const std::weak_ptr<I_Edge>& right
			) {
				return left.lock()->GetPriority() < right.lock()->GetPriority();	//優先順位が低い順にソート
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

			void SelecterBase::ChangeCurrentNode(const std::shared_ptr<I_Node>& node) {
				for (auto data : m_transitionDatas) {
					if (data->node.lock() == node) {
						m_currentTransitionData = data;
						break;
					}
				}
			}
			

			//--------------------------------------------------------------------------------------
			/// ビヘイビアセレクターの改良型(開発途中のため、使用禁止)
			//--------------------------------------------------------------------------------------

			Selecter::Selecter():
				Selecter(SelectType::Priority)
			{}

			Selecter::Selecter(const SelectType selectType) :
				m_selectType(selectType)
			{ }

			void Selecter::OnStart() {
				//更新するノードを決める。
				ChangeCurrentNode(SearchCurrentNode());
			}

			bool Selecter::OnUpdate() {
				if (HasCurrentNode()) {
					return GetCurrentNode()->OnUpdate();
				}

				return true;
			}

			void Selecter::OnExit() {
				//更新中のノードをnullptrにする。(関数内で終了処理も呼べる。)
				ChangeCurrentNode(nullptr);
			}


			std::shared_ptr<I_Node> Selecter::SearchCurrentNode() const {
				switch (m_selectType)
				{
				case basecross::maru::Behavior::SelectType::Priority:
					return SearchFirstPriorityNode();
				case basecross::maru::Behavior::SelectType::Random:
					return SearchRandomNode();
				}

				return nullptr;
			}

			std::shared_ptr<I_Node> Selecter::SearchFirstPriorityNode() const {
				//遷移先ノードが空ならnullptr
				if (IsEmptyTransitionNodes()) {
					return nullptr;
				}

				auto transitionEdges = m_transitionEdges;		//メンバをソートするとconstにできないため、一時変数化
				std::sort(transitionEdges.begin(), transitionEdges.end(), &SortEdges);	//昇順ソート

				return transitionEdges[0].lock()->GetToNode();	//一番優先度が高いノードを返す。
			}

			std::shared_ptr<I_Node> Selecter::SearchRandomNode() const {
				auto randomEdge = maru::MyRandom::RandomArray(m_transitionEdges);
				return randomEdge.lock()->GetToNode();
			}

			void Selecter::ChangeCurrentNode(const std::shared_ptr<I_Node>& node) {
				//現在のノードの終了処理をする。
				if (auto currentNode = GetCurrentNode()) {
					currentNode->OnExit();
				}

				m_currentNode = node;

				//変更するノードがnullでなかったら、開始処理をする。
				if (node) {
					node->OnStart();
				}
			}

			//--------------------------------------------------------------------------------------
			/// アクセッサ
			//--------------------------------------------------------------------------------------

			void Selecter::SetFromEdge(const std::shared_ptr<I_Edge>& fromEdge) noexcept {
				m_fromEdge = fromEdge;
			}

			std::shared_ptr<I_Edge> Selecter::GetFromEdge() const noexcept {
				return m_fromEdge.lock();
			}

			void Selecter::AddTransitionEdge(const std::shared_ptr<I_Edge>& edge) {
				m_transitionEdges.push_back(edge);
			}

			bool Selecter::IsEmptyTransitionNodes() const {
				return static_cast<int>(m_transitionEdges.size()) == 0;
			}

			std::shared_ptr<I_Node> Selecter::GetCurrentNode() const {
				return m_currentNode.lock();
			}

		}
	}
}