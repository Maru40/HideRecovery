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
			class Selecter;

			//--------------------------------------------------------------------------------------
			/// ビヘイビアのインターフェース
			//--------------------------------------------------------------------------------------
			class I_Behavior {

			};

			//--------------------------------------------------------------------------------------
			/// ビヘイビア
			//--------------------------------------------------------------------------------------
			template<class EnumType>
			class BehaviorTree : public I_Behavior
			{
			public:
				using NodeMap = std::unordered_map<EnumType, std::shared_ptr<I_Node>>;
				using SelecterMap = std::unordered_map<EnumType, std::shared_ptr<Selecter>>;
				using TaskMap = std::unordered_map<EnumType, std::shared_ptr<I_Task>>;

				using EdgesMap = std::unordered_map<EnumType, std::vector<std::shared_ptr<I_Edge>>>;

			private:
				EnumType m_firstNodeType = EnumType(0);	//初回ノードタイプ
				std::weak_ptr<I_Node> m_currentNode;	//現在積まれているタスク

				NodeMap m_nodeMap;						//定義したノード
				SelecterMap m_selecterMap;				//定義したセレクター
				TaskMap m_taskMap;						//定義したタスク

				std::stack<std::weak_ptr<I_Node>> m_currentStack;	//現在積まれているノードスタック

				EdgesMap m_edgesMap;	//遷移エッジ
	
			private:
				/// <summary>
				/// ノードの追加
				/// </summary>
				/// <param name="type">ノードタイプ</param>
				/// <param name="node">ノード</param>
				std::shared_ptr<I_Node> AddNode(const EnumType type, const std::shared_ptr<I_Node>& node) {
					node->SetIndex(static_cast<int>(type));
					m_nodeMap[type] = node;
					return node;
				}

				/// <summary>
				/// セレクターとエッジの結合
				/// </summary>
				/// <param name="edge"></param>
				void Union(const std::shared_ptr<I_Edge>& edge) {
					std::shared_ptr<I_Node> fromNode = edge->GetFromNode();
					std::shared_ptr<I_Node> toNode = edge->GetToNode();

					//エッジの手前ノードがセレクターであるなら、遷移先ノード情報を設定
					if (auto fromSelecter = GetSelecter(fromNode->GetType<EnumType>())) {
						fromSelecter->AddTransitionEdge(edge);
					}

					//遷移先のノードがセレクターなら、手前ノードを登録する。
					if (auto toSelecter = GetSelecter(toNode->GetType<EnumType>())) {
						toSelecter->SetFromEdge(edge);
					}
				}

				/// <summary>
				///	初期ノードのリセット
				/// </summary>
				void ResetFirstNode() {
					for (auto& edge : GetEdges(m_firstNodeType)) {
						edge->GetToNode()->SetState(BehaviorState::Inactive);
					}
				}

				/// <summary>
				/// カレントスタックに追加する。
				/// </summary>
				/// <param name="node"></param>
				void AddCurrentStack(const std::shared_ptr<I_Node>& node) {
					//ノードが存在するなら追加処理をする。
					if (node) {
						node->OnStart();
						node->SetState(BehaviorState::Running);
						m_currentStack.push(node);
					}
				}

				/// <summary>
				/// カレントスタックからポップする。
				/// </summary>
				void PopCurrentStack() {
					if (m_currentStack.empty()) {	//スタックが空なら処理をしない。
						return;
					}

					auto node = m_currentStack.top().lock();
					if (node) {
						node->OnExit();		//ノードの終了判定処理
						node->SetState(BehaviorState::Completed);
					}

					m_currentStack.pop();	//スタックからポップ
				}

			public:
				virtual ~BehaviorTree() = default;

				/// <summary>
				/// 現在のタスクのタイプ
				/// </summary>
				/// <returns>現在のタスクタイプ</returns>
				EnumType GetCurrentType() const { return m_currentNode.lock() ? m_currentNode.lock()->GetType<EnumType>() : EnumType(0); }

				/// <summary>
				/// 現在のノードを設定
				/// </summary>
				/// <param name="node">現在のノード</param>
				void SetCurrentNode(const std::shared_ptr<I_Node>& node) { m_currentNode = node; }

				/// <summary>
				/// 現在のノードを取得
				/// </summary>
				/// <returns>現在のノード</returns>
				std::shared_ptr<I_Node> GetCurrentNode() const { return m_currentNode.lock(); }

				/// <summary>
				/// そのノードが存在するかどうか
				/// </summary>
				/// <param name="type">確認したいタイプ</param>
				/// <returns>ノードが存在するならtrue</returns>
				bool HasNode(const EnumType type) const { return static_cast<int>(m_nodeMap.count(type)) != 0; }

				/// <summary>
				/// ノードの取得
				/// </summary>
				/// <param name="type">ノードのタイプ</param>
				/// <returns>取得したノード</returns>
				std::shared_ptr<I_Node> GetNode(const EnumType type) const {
					return m_nodeMap.count(type) != 0 ? m_nodeMap.at(type) : nullptr;
				}

				/// <summary>
				/// 渡したノードの遷移先ノードを取得
				/// </summary>
				/// <param name="node">遷移先を取得したいノード</param>
				/// <returns>最優先の遷移先ノード</returns>
				std::shared_ptr<I_Node> GetTransitionNode(const std::shared_ptr<I_Node>& node) {
					//将来的にシーケンサ用の処理が入る。
					auto selecter = GetSelecter(node->GetType<EnumType>());
					if (!selecter) {
						return nullptr;
					}

					//セレクターのカレントノードを検索して取得
					return selecter->SearchCurrentNode();
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
				void AddSelecter(const EnumType type, const std::shared_ptr<Selecter>& selecter = std::make_shared<Selecter>()) {
					m_selecterMap[type] = selecter;
					AddNode(type, selecter);
				}

				/// <summary>
				/// セレクターの取得
				/// </summary>
				/// <returns>セレクター</returns>
				std::shared_ptr<Selecter> GetSelecter(const EnumType type) const {
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
				/// デコレータの設定
				/// </summary>
				/// <param name="type">設定したいノ―ドタイプ</param>
				/// <param name="decorator">設定したいデコレータ</param>
				bool AddDecorator(const EnumType type, const std::shared_ptr<I_Decorator>& decorator) {
					if (!HasNode(type)) {
						return false;
					}

					auto node = GetNode(type);
					node->AddDecorator(decorator);

					return true;
				}

				/// <summary>
				/// デコレータの設定
				/// </summary>
				template<class T, class... Ts,
					std::enable_if_t<
						std::is_constructible_v<T, Ts...> &&	//コンストラクタの引数
						std::is_base_of_v<I_Decorator, T>,		//基底クラスの制限
					std::nullptr_t> = nullptr
				>
				bool AddDecorator(const EnumType type, Ts&&... params) {
					return AddDecorator(type, std::make_shared<T>(params...));
				}

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

				std::shared_ptr<I_Node> DecoratorsUpdate() {
					auto copyStack = m_currentStack;
					while (!copyStack.empty()) {	//スタックが空になるまで
						auto node = copyStack.top().lock();
						if (!node->CanUpdate()) {	//ノードが更新可能でなかったら
							return node;
						}

						copyStack.pop();
					}

					return nullptr;
				}

				/// <summary>
				/// 遷移先のノードが見つかるまで、スタックを巻き戻す。
				/// </summary>
				/// <param name="node">確認したいノード</param>
				/// <returns></returns>
				std::shared_ptr<I_Node> ReverseStack(const std::shared_ptr<I_Node>& node) {
					if (node == GetNode(m_firstNodeType)) {
						ResetFirstNode();
					}

					auto selecter = GetSelecter(node->GetType<EnumType>());
					if (!selecter) {	//セレクターでないなら前のノードに戻る。
						PopCurrentStack();
						return ReverseStack(m_currentStack.top().lock());
					}

					auto nextNode = selecter->SearchCurrentNode();
					if (!nextNode) {	//ノードが存在しないなら、手前のノードに戻る。
						PopCurrentStack();
						return ReverseStack(m_currentStack.top().lock());
					}

					return nextNode;
				}

				/// <summary>
				/// 再起処理をして、遷移先のノードを取得する。
				/// </summary>
				/// <returns>一番優先度の高いノード</returns>
				std::shared_ptr<I_Node> Recursive_TransitionNode(const std::shared_ptr<I_Node>& node) {
					if (!node) {
						return nullptr;
					}

					//Taskノードなら、末端ノードとなる。
					if (HasTask(node->GetType<EnumType>())) {
						AddCurrentStack(node);	//最終ノードをスタックに入れる。
						return node;			//末端ノードが確定
					}

					//遷移先が存在するなら、スタックに追加して再起処理
					if (auto transitionNode = GetTransitionNode(node)) {
						AddCurrentStack(node);	//スタックに積む。
						//一番優先順位の高いノードを取得する。
						return Recursive_TransitionNode(transitionNode);
					}

					//遷移先がないため、Popして戻る。
					return ReverseStack(node);
				}

				/// <summary>
				/// 引数で渡したノードの手前ノードまでスタックを戻す
				/// </summary>
				/// <param name="targetNode">戻りたいノード</param>
				void ReverseTargetBeforeStack(const std::shared_ptr<I_Node>& targetNode) {
					if (m_currentStack.empty()) {
						return;
					}

					auto currentNode = m_currentStack.top().lock();
					if (currentNode == targetNode) {
						PopCurrentStack();	//ストップした手前のノードまで戻って終了
						return;
					}

					PopCurrentStack();		//スタックをポップ
					ReverseTargetBeforeStack(targetNode);	//再起処理
				}

				/// <summary>
				/// 遷移するときの判断開始位置のノードを取得する。
				/// </summary>
				std::shared_ptr<I_Node> GetTransitionStartNode() {
					if (m_currentStack.size() == 0) {	
						return GetNode(m_firstNodeType);
					}
					
					return ReverseStack(m_currentStack.top().lock());
				}

				/// <summary>
				/// 遷移処理
				/// </summary>
				void Transition() {
					//優先度の一番高いノードに遷移
					auto node = Recursive_TransitionNode(GetTransitionStartNode());
					SetCurrentNode(node);	//カレントノードの設定
				}

			public:
				/// <summary>
				/// 更新処理
				/// </summary>
				void OnUpdate() {
					if (IsEmpty()) {
						return;
					}

					//監視が必要なデコレータの更新
					auto stopNode = DecoratorsUpdate();
					if (stopNode) {	//戻り値が存在するなら、更新不能なノードが存在した。
						//ストップしたノードの手前ノードまで戻る。
						ReverseTargetBeforeStack(stopNode);
						Transition();	//遷移
					}

					bool isEndTaskUpdate = TaskUpdate();	//ノードの更新

					//ノードが終了したら、遷移
					if (isEndTaskUpdate) {
						Transition();	//遷移
					}
				}

			};

		}

	}
}