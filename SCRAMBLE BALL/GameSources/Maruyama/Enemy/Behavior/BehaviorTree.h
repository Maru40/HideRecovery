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
			/// デコレータのインターフェース
			//--------------------------------------------------------------------------------------
			class I_Decorator {
			public:
				virtual ~I_Decorator() = default;

				/// <summary>
				/// 遷移できるかどうか
				/// </summary>
				/// <returns>遷移できるならtrue</returns>
				virtual bool CanTransition() const = 0;
			};

			//--------------------------------------------------------------------------------------
			/// 優先度管理のインターフェース
			//--------------------------------------------------------------------------------------
			class I_PriorityController {
			public:
				virtual ~I_PriorityController() = default;

				/// <summary>
				/// 優先度の取得
				/// </summary>
				/// <returns>優先度</returns>
				virtual float GetPriority() const = 0;
			};

			//--------------------------------------------------------------------------------------
			/// 優先度管理の基底クラス
			//--------------------------------------------------------------------------------------
			class PriorityControllerBase : public I_PriorityController {
				float m_priority = 0;

			public:
				virtual ~PriorityControllerBase() = default;

				float GetPriority() const override { return m_priority; }
			};

			//--------------------------------------------------------------------------------------
			/// ビヘイビアノードのインターフェース
			//--------------------------------------------------------------------------------------
			class I_Node {
			public:
				virtual ~I_Node() = default;

				/// <summary>
				/// インデックスのセット
				/// </summary>
				virtual void SetIndex(const int index) noexcept = 0;

				/// <summary>
				/// インデックスの取得
				/// </summary>
				/// <returns></returns>
				virtual int GetIndex() const noexcept = 0;

				/// <summary>
				/// 遷移できるかどうか
				/// </summary>
				/// <returns>遷移できるならtrue</returns>
				virtual bool CanTransition() const = 0;

				/// <summary>
				/// デコレータの追加
				/// </summary>
				virtual void AddDecorator(const std::shared_ptr<I_Decorator>& decorator) = 0;

				/// <summary>
				/// デコレータがあるかどうか
				/// </summary>
				/// <returns>デコレータがEmptyならtrue</returns>
				virtual bool IsDecoratorEmpty() const = 0;
			};

			//--------------------------------------------------------------------------------------
			/// ビヘイビアノードの基底クラス
			//--------------------------------------------------------------------------------------
			class NodeBase : public I_Node
			{
				int m_index = 0;										//ノードインデックス
				std::vector<std::shared_ptr<I_Decorator>> m_decorators;	//デコレータ配列

			public:
				virtual ~NodeBase() = default;

				void SetIndex(const int index) noexcept override { m_index = index; }

				int GetIndex() const noexcept override { return m_index; }

				bool CanTransition() const override;

				void AddDecorator(const std::shared_ptr<I_Decorator>& decorator) override;

				bool IsDecoratorEmpty() const override;
			};

			//--------------------------------------------------------------------------------------
			/// ビヘイビアタスクのインターフェース
			//--------------------------------------------------------------------------------------
			class I_Task : public NodeBase {
			public:
				virtual ~I_Task() = default;

				virtual void OnStart() = 0;
				virtual bool OnUpdate() = 0;
				virtual void OnExit() = 0;
			};

			//--------------------------------------------------------------------------------------
			/// ビヘイビアセレクターの遷移先ノードデータ
			//--------------------------------------------------------------------------------------
			struct Selecter_TransitionNodeData {
				std::shared_ptr<I_PriorityController> priorityController;	//優先度コントローラー
				std::weak_ptr<I_Node> node;									//ノード

				Selecter_TransitionNodeData(
					const std::shared_ptr<I_PriorityController>& priorityController,
					const std::shared_ptr<I_Node>& node
				);

				virtual ~Selecter_TransitionNodeData() = default;
			};

			//--------------------------------------------------------------------------------------
			/// ビヘイビアセレクターのインターフェース
			//--------------------------------------------------------------------------------------
			class I_Selecter : public NodeBase {
			public:
				using TransitionNodeData = Selecter_TransitionNodeData;

			public:
				virtual ~I_Selecter() = default;

				/// <summary>
				/// 手前のノードの設定
				/// </summary>
				/// <param name="node">手前のノード</param>
				virtual void SetFromNode(const std::shared_ptr<I_Node>& node) = 0;

				/// <summary>
				/// 手前のノードの取得
				/// </summary>
				/// <returns>手前のノード</returns>
				virtual std::shared_ptr<I_Node> GetFromNode() const = 0;

				/// <summary>
				/// 遷移先ノードの追加
				/// </summary>
				/// <param name="priorityController">優先度管理</param>
				/// <param name="node">遷移先ノード</param>
				virtual void AddTransitionNode(
					const std::shared_ptr<I_PriorityController>& priorityController,
					const std::shared_ptr<I_Node>& node
				) = 0;

				/// <summary>
				/// 一番優先度高いノードの取得
				/// </summary>
				/// <returns></returns>
				virtual std::shared_ptr<I_Node> GetFirstPriorityNode() const = 0;

				/// <summary>
				/// 遷移先ノードが空かどうか
				/// </summary>
				/// <returns>遷移先ノードが空ならtrue</returns>
				virtual bool IsEmptyTransitionNodes() const = 0;

			};

			//--------------------------------------------------------------------------------------
			/// ビヘイビアセレクターの基底クラス
			//--------------------------------------------------------------------------------------

			class SelecterBase : public I_Selecter
			{
				std::weak_ptr<I_Node> m_fromNode;									//自分の手前に存在するノード
				std::vector<std::shared_ptr<TransitionNodeData>> m_transitionDatas;	//自分の遷移先ノード群(優先度)

			public:
				SelecterBase(const std::shared_ptr<I_Node>& fromNode);	//コンストラクタ

				virtual ~SelecterBase() = default;	//デストラクタ

				void SetFromNode(const std::shared_ptr<I_Node>& node) { m_fromNode = node; }
				
				std::shared_ptr<I_Node> GetFromNode() const { return m_fromNode.lock(); }

				void AddTransitionNode(
					const std::shared_ptr<I_PriorityController>& priorityController,
					const std::shared_ptr<I_Node>& node
				) {
					m_transitionDatas.push_back(std::make_shared<TransitionNodeData>(priorityController, node));
				}

				std::shared_ptr<I_Node> GetFirstPriorityNode() const;

				bool IsEmptyTransitionNodes() const;

			};

			//--------------------------------------------------------------------------------------
			/// ビヘイビアのエッジインターフェース
			//--------------------------------------------------------------------------------------
			class I_Edge {
			public:
				virtual ~I_Edge() = default;

				/// <summary>
				/// 手間のノードを設定
				/// </summary>
				/// <param name="node">手前のノード</param>
				virtual void SetFromNode(const std::shared_ptr<I_Node>& node) = 0;

				/// <summary>
				/// 手前のノードを取得
				/// </summary>
				/// <returns>手前のノード</returns>
				virtual std::shared_ptr<I_Node> GetFromNode() const = 0;

				/// <summary>
				/// 先のノードの設定
				/// </summary>
				/// <param name="node">先のノード</param>
				virtual void SetToNode(const std::shared_ptr<I_Node>& node) = 0;

				/// <summary>
				/// 先のノードを取得
				/// </summary>
				/// <returns>先のノード</returns>
				virtual std::shared_ptr<I_Node> GetToNode() const = 0;
			};

			//--------------------------------------------------------------------------------------
			/// ビヘイビアのエッジの基底クラス
			//--------------------------------------------------------------------------------------
			class EdgeBase : public I_Edge
			{
				std::weak_ptr<I_Node> m_fromNode;	//自分の手前のノード
				std::weak_ptr<I_Node> m_toNode;		//自分の先のノード

			public:
				virtual ~EdgeBase() = default;

				EdgeBase(const std::shared_ptr<I_Node>& fromNode, const std::shared_ptr<I_Node>& toNode) :
					m_fromNode(fromNode),
					m_toNode(toNode)
				{ }

				void SetFromNode(const std::shared_ptr<I_Node>& node) override { m_fromNode = node; }

				std::shared_ptr<I_Node> GetFromNode() const override { return m_fromNode.lock(); }

				void SetToNode(const std::shared_ptr<I_Node>& node) override { m_toNode = node; }

				std::shared_ptr<I_Node> GetToNode() const override { return m_toNode.lock(); }
			};

			//--------------------------------------------------------------------------------------
			/// ビヘイビア
			//--------------------------------------------------------------------------------------
			template<class EnumType>
			class BehaviorTree
			{
				EnumType m_currentType = EnumType(0);	//現在のタスク

				std::unordered_map<EnumType, std::shared_ptr<I_Selecter>> m_selecterMap;		//定義したセレクター
				std::unordered_map<EnumType, std::shared_ptr<I_Task>> m_taskMap;				//定義したタスク
				std::unordered_map<EnumType, std::shared_ptr<I_Node>> m_nodeMap;				//定義したノード
				std::unordered_map<EnumType, std::vector<std::shared_ptr<I_Edge>>> m_edgesMap;	//定義したエッジ

			private:
				/// <summary>
				/// ノードの追加
				/// </summary>
				/// <param name="type">ノードタイプ</param>
				/// <param name="node">ノード</param>
				void AddNode(const EnumType type, const std::shared_ptr<I_Node>& node) {
					m_nodeMap[type] = node;
				}

			public:
				virtual ~BehaviorTree() = default;

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
				/// 指定したタイプのSelecterを持っているかどうか
				/// </summary>
				/// <param name="type">指定タイプ</param>
				/// <returns>持っているならtrue</returns>
				bool HasSelecter(const EnumType type) {
					return static_cast<int>(m_selecterMap.count(type)) != 0;	//0でなかったら
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
				/// タスクが定義されているかどうか
				/// </summary>
				/// <param name="type">タスクタイプ</param>
				/// <returns>タスクが定義されていたらtrue</returns>
				bool HasTask(const EnumType type) const {
					return static_cast<int>(m_taskMap.count(type)) != 0;
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