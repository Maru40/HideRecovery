/*!
@file BehaviorTree.h
@brief BehaviorTreeなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Astar/SparseGraph.h"

enum class TestEnumTask : std::uint8_t
{
	Task
};

namespace basecross {
	namespace maru {

		namespace Behavior {
			//--------------------------------------------------------------------------------------
			/// 前方宣言
			//--------------------------------------------------------------------------------------
			class I_Node;
			class NodeBase;

			class I_Edge;
			class EdgeBase;

			class I_Decorator;
			class I_PriorityController;
			class I_Task;

			class I_Selecter;
			class I_SelecterBase;

			//--------------------------------------------------------------------------------------
			/// ビヘイビア
			//--------------------------------------------------------------------------------------
			template<class EnumType>
			class BehaviorTree
			{
			public:
				using EdgesMap = std::unordered_map<EnumType, std::vector<std::shared_ptr<I_Edge>>>;

			private:
				EnumType m_currentType = EnumType(0);	//現在のタスク

				std::unordered_map<EnumType, std::shared_ptr<I_Node>> m_nodeMap;			//定義したノード
				std::unordered_map<EnumType, std::shared_ptr<I_Selecter>> m_selecterMap;	//定義したセレクター
				std::unordered_map<EnumType, std::shared_ptr<I_Task>> m_taskMap;			//定義したタスク

				EdgesMap m_edgesMap;	//遷移エッジ

			private:
				/// <summary>
				/// ノードの追加
				/// </summary>
				/// <param name="type">ノードタイプ</param>
				/// <param name="node">ノード</param>
				int AddNode(const EnumType type, const std::shared_ptr<I_Node>& node) override {
					int index = SparseGraph::AddNode(node);
					m_nodeMap[type] = node;

					return index;
				}

			public:
				virtual ~BehaviorTree() = default;

				/// <summary>
				/// 指定したタイプのSelecterを持っているかどうか
				/// </summary>
				/// <param name="type">指定タイプ</param>
				/// <returns>持っているならtrue</returns>
				bool HasSelecter(const EnumType type) {
					return static_cast<int>(m_selecterMap.count(type)) != 0;	//0でなかったら
				}

				/// <summary>
				/// セレクターの追加
				/// </summary>
				/// <param name="type">ノードタイプ</param>
				/// <param name="selecter">セレクター</param>
				void AddSelecter(const EnumType type, const std::shared_ptr<I_Selecter>& selecter) {
					m_selecterMap[type] = selecter;
					AddNode(type, selecter);
				}

				/// <summary>
				/// セレクターの取得
				/// </summary>
				/// <returns>セレクター</returns>
				std::shared_ptr<I_Selecter> GetSelecter(const EnumType type) const {
					return HasSelecter(type) ? m_selecterMap.at(type) : nullptr;	//持っていないならnullptrを返す。
				}

				/// <summary>
				/// タスクが定義されているかどうか
				/// </summary>
				/// <param name="type">タスクタイプ</param>
				/// <returns>タスクが定義されていたらtrue</returns>
				bool HasTask(const EnumType type) const {
					return static_cast<int>(m_taskMap.count(type)) != 0;
				}

				/// <summary>
				/// タスクの追加
				/// </summary>
				/// <param name="type">ノードタイプ</param>
				/// <param name="node">タスク</param>
				void AddTask(const EnumType type, std::shared_ptr<I_Task>& task) {
					m_taskMap[type] = task;
					AddNode(type, task);
				}

				/// <summary>
				/// タスクの取得
				/// </summary>
				/// <param name="type">ノードタイプ</param>
				std::shared_ptr<I_Task> GetTask(const EnumType type) const {
					return m_taskMap.at(type);
				}

				/// <summary>
				/// 現在のノードを取得
				/// </summary>
				std::shared_ptr<I_Task> GetCurrentTask() const {
					return m_taskMap.at(m_currentType);
				}

				/// <summary>
				/// エッジの追加
				/// </summary>
				/// <param name="fromType"></param>
				/// <param name="toType"></param>
				void AddEdge(const EnumType fromType, const EnumType toType) {
					AddEdge<EdgeBase>(fromType, toType);
				}

				/// <summary>
				/// エッジの追加
				/// </summary>
				template<class T, class... Ts,
					std::enable_if_t<std::is_constructible_v<T, std::shared_ptr<I_Edge>&, Ts...>, std::nullptr_t> = nullptr>
				void AddEdge(Ts&&... params) {
					std::shared_ptr<I_Edge> newEdge = std::make_shared<T>(params...);

					std::shared_ptr<I_Node> fromNode = newEdge->GetFromNode();
					std::shared_ptr<I_Node> toNode = newEdge->GetToNode();
					
					auto type = static_cast<EnumType>(fromNode->GetIndex());

					//そのセレクターがあるなら、タスクに先のノード情報を設定
					if (auto selecter = GetSelecter(type)) {
						selecter->SetFromNode(fromNode);
						selecter->AddTransitionNode(toNode);
					}

					auto fromType = static_cast<EnumType>(fromNode->GetIndex());
					m_edgesMap[fromType].push_back(newEdge);
				}

				/// <summary>
				/// 空の状態かどうか
				/// </summary>
				/// <returns>空の状態ならtrue</returns>
				bool IsEmpty() const { return static_cast<int>(m_taskMap.size()) == 0; }

			public:
				/// <summary>
				/// 更新処理
				/// </summary>
				void OnUpdate() {
					if (IsEmpty()) {
						return;
					}

					bool isEndTaskUpdate = TaskUpdate();

					//ノードが終了したら、最初に戻る

				}

			private:
				/// <summary>
				/// ノードの更新
				/// </summary>
				/// <returns>ノードの更新が終了したならtrue</returns>
				bool TaskUpdate() {
					auto currentTask = GetCurrentTask();
					if (currentTask) {
						return currentTask->OnUpdate();
					}

					return true;
				}

			};

		}

	}
}