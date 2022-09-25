/*!
@file I_Selecter.h
@brief I_Selecterなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "I_Node.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			//--------------------------------------------------------------------------------------
			/// 前方宣言
			//--------------------------------------------------------------------------------------
			class I_PriorityController;

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
				virtual ~SelecterBase() = default;	//デストラクタ

				void SetFromNode(const std::shared_ptr<I_Node>& node) override { m_fromNode = node; }

				std::shared_ptr<I_Node> GetFromNode() const override { return m_fromNode.lock(); }

				void AddTransitionNode(
					const std::shared_ptr<I_PriorityController>& priorityController,
					const std::shared_ptr<I_Node>& node
				) override {
					m_transitionDatas.push_back(std::make_shared<TransitionNodeData>(priorityController, node));
				}

				/// <summary>
				/// 最優先のノードを取得
				/// </summary>
				/// <returns>最優先ノード</returns>
				std::shared_ptr<I_Node> GetFirstPriorityNode() const override;

				bool IsEmptyTransitionNodes() const override;

			};

		}
	}
}