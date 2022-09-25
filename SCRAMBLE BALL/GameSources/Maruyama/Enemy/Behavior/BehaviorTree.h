/*!
@file BehaviorTree.h
@brief BehaviorTreeなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Astar/SparseGraph.h"

#include "Interface/I_Node.h"
#include "Interface/I_Edge.h"
#include "Interface/I_Task.h"
#include "Interface/I_Selecter.h"
#include "Interface/I_Decorator.h"
#include "Interface/I_PriorityController.h"

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
				//EnumType m_currentType = EnumType(0);	//現在のタスク
				EnumType m_firstNodeType = EnumType(0);	//初回ノードタイプ
				std::weak_ptr<I_Node> m_currentNode;	//現在積まれているタスク

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
				std::shared_ptr<I_Node> AddNode(const EnumType type, const std::shared_ptr<I_Node>& node) {
					m_nodeMap[type] = node;

					return node;
				}

				void Union(const std::shared_ptr<I_Edge>& edge) {
					std::shared_ptr<I_Node> fromNode = edge->GetFromNode();
					std::shared_ptr<I_Node> toNode = edge->GetToNode();

					//そのセレクターがあるなら、タスクに先のノード情報を設定
					if (auto selecter = GetSelecter(fromNode->GetType<EnumType>())) {
						selecter->SetFromNode(fromNode);
						selecter->AddTransitionNode(edge->GetPriorityContorller(), toNode);
					}
				}


			public:
				virtual ~BehaviorTree() = default;

				/// <summary>
				/// 現在のタスクのタイプ
				/// </summary>
				/// <returns>現在のタスクタイプ</returns>
				EnumType GetCurrentType() const { return m_currentNode.lock() ? m_currentNode.lock()->GetType<EnumType>() : EnumType(0); }

				std::shared_ptr<I_Node> GetCurrentNode() const { return m_currentNode.lock(); }

				bool HasNode(const EnumType type) const { return static_cast<int>(m_nodeMap.count(type)) != 0; }

				std::shared_ptr<I_Node> GetNode(const EnumType type) const {
					return m_nodeMap.count(type) != 0 ? m_nodeMap.at(type) : nullptr;
				}

				/// <summary>
				/// 最優先の遷移先ノードの取得
				/// </summary>
				/// <param name="type">開始ノード</param>
				/// <returns>最優先の遷移先ノード</returns>
				std::shared_ptr<I_Node> CalculateFirstPriorityNode(const EnumType type) {
					//初期ノードがから伸びるエッジを取得
					std::vector<std::shared_ptr<I_Edge>> edges = GetEdges(type);

					////優先度順に並び変える
					auto sortEvent = [](const std::shared_ptr<I_Edge>& right, const std::shared_ptr<I_Edge>& left) {
						return right->GetPriority() < left->GetPriority();	//優先順位が低い順にソート
					};
					std::sort(edges.begin(), edges.end(), sortEvent);

					return edges[0]->GetToNode();	//優先順位の高いノードを取得
				}

				/// <summary>
				/// 指定したタイプのSelecterを持っているかどうか
				/// </summary>
				/// <param name="type">指定タイプ</param>
				/// <returns>持っているならtrue</returns>
				bool HasSelecter(const EnumType type) const { return static_cast<int>(m_selecterMap.count(type)) != 0; }

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
				void AddTask(const EnumType type, const std::shared_ptr<I_Task>& task) {
					m_taskMap[type] = task;
					AddNode(type, task);
				}

				/// <summary>
				/// タスクの取得
				/// </summary>
				/// <param name="type">ノードタイプ</param>
				std::shared_ptr<I_Task> GetTask(const EnumType type) const {
					return HasTask(type) ? m_taskMap.at(type) : nullptr;
				}

				/// <summary>
				/// 現在のノードを取得
				/// </summary>
				std::shared_ptr<I_Task> GetCurrentTask() const {
					return dynamic_pointer_cast<I_Task>(m_currentNode.lock());
				}

				/// <summary>
				/// エッジの追加
				/// </summary>
				template<class T, class... Ts,
					std::enable_if_t<std::is_constructible_v<T, Ts...>, std::nullptr_t> = nullptr>
				void AddEdge(Ts&&... params) {
					std::shared_ptr<I_Edge> newEdge = std::make_shared<T>(params...);
					Union(newEdge);

					auto fromType = newEdge->GetFromNode()->GetType<EnumType>();
					m_edgesMap[fromType].push_back(newEdge);
				}

				/// <summary>
				/// エッジの追加
				/// </summary>
				/// <param name="fromType"></param>
				/// <param name="toType"></param>
				void AddEdge(const EnumType fromType, const EnumType toType, const std::shared_ptr<I_PriorityController>& priorityController) {
					AddEdge<EdgeBase>(GetNode(fromType), GetNode(toType), priorityController);
				}

				/// <summary>
				/// 引数のタイプから伸びるエッジ配列の取得
				/// </summary>
				/// <param name="type">タイプ</param>
				/// <returns>引数のタイプから伸びるエッジ配列</returns>
				std::vector<std::shared_ptr<I_Edge>> GetEdges(const EnumType type) const {
					if (!HasEdges(type)) {	//そのタイプのエッジが存在しないなら空の配列を返す。
						return std::vector<std::shared_ptr<I_Edge>>();
					}

					return m_edgesMap.at(type);
				}

				/// <summary>
				/// エッジを持っているかどうか
				/// </summary>
				/// <param name="type">エッジのFromタイプ</param>
				/// <returns>エッジがあるならtrue</returns>
				bool HasEdges(const EnumType type) const { return static_cast<int>(m_edgesMap.count(type)) != 0; }

				/// <summary>
				/// 空の状態かどうか
				/// </summary>
				/// <returns>空の状態ならtrue</returns>
				bool IsEmpty() const { return static_cast<int>(m_taskMap.size()) == 0; }

				/// <summary>
				/// 開始ノードタイプの設定
				/// </summary>
				/// <param name="type">開始ノードタイプ</param>
				void SetFirstNodeType(const EnumType type) { m_firstNodeType = type; }

				/// <summary>
				/// 開始ノードの取得
				/// </summary>
				/// <returns>開始ノード</returns>
				EnumType GetFirstNodeType() const { return m_firstNodeType; }

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
					if (isEndTaskUpdate) {
						Transition();	//遷移
					}
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

				//再起処理をして、遷移先のノードを取得する。
				std::shared_ptr<I_Node> Recursive_TransitionNode(const std::shared_ptr<I_Node>& node) {
					//エッジが存在しないならnodeを生成する。
					if (!HasEdges(node->GetType<EnumType>())) {	
						return node;
					}

					//一番優先順位の高いノードを取得する。
					return Recursive_TransitionNode(CalculateFirstPriorityNode(node->GetType<EnumType>()));
				}

				/// <summary>
				/// 遷移処理
				/// </summary>
				void Transition() {
					//優先度の一番高いノードに遷移
					auto node = Recursive_TransitionNode(GetNode(m_firstNodeType));

					m_currentNode = node;
				}

			};

		}

	}
}