/*!
@file I_Edge.h
@brief I_Edgeなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			//--------------------------------------------------------------------------------------
			/// 前方宣言
			//--------------------------------------------------------------------------------------
			class I_Node;
			class I_PriorityController;

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

				/// <summary>
				/// 優先度の設定
				/// </summary>
				/// <param name="priority">優先度</param>
				virtual void SetPriority(const float priority) = 0;

				/// <summary>
				/// 優先度の取得
				/// </summary>
				/// <returns>優先度</returns>
				virtual float GetPriority() const = 0;

				virtual void SetPriorityController(const std::shared_ptr<I_PriorityController>& priorityController) = 0;

				virtual std::shared_ptr<I_PriorityController> GetPriorityContorller() = 0;
			};

			//--------------------------------------------------------------------------------------
			/// ビヘイビアのエッジの基底クラス
			//--------------------------------------------------------------------------------------
			class EdgeBase : public I_Edge
			{
				std::weak_ptr<I_Node> m_fromNode;	//自分の手前のノード
				std::weak_ptr<I_Node> m_toNode;		//自分の先のノード
				std::shared_ptr<I_PriorityController> m_priorityContorller;	//優先度管理

			public:
				EdgeBase(
					const std::shared_ptr<I_Node>& fromNode, 
					const std::shared_ptr<I_Node>& toNode
				);

				EdgeBase(
					const std::shared_ptr<I_Node>& fromNode,
					const std::shared_ptr<I_Node>& toNode,
					const std::shared_ptr<I_PriorityController> priorityController
				);

				virtual ~EdgeBase() = default;

				void SetFromNode(const std::shared_ptr<I_Node>& node) override { m_fromNode = node; }

				std::shared_ptr<I_Node> GetFromNode() const override { return m_fromNode.lock(); }

				void SetToNode(const std::shared_ptr<I_Node>& node) override { m_toNode = node; }

				std::shared_ptr<I_Node> GetToNode() const override { return m_toNode.lock(); }

				void SetPriority(const float priority) override;

				float GetPriority() const override;

				virtual void SetPriorityController(const std::shared_ptr<I_PriorityController>& priorityController) override;

				virtual std::shared_ptr<I_PriorityController> GetPriorityContorller() override;
			};

		}
	}
}