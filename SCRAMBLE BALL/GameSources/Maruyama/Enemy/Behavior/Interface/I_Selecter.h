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
			class I_Edge;
			class I_PriorityController;

			//--------------------------------------------------------------------------------------
			/// ビヘイビアセレクターのセレクトタイプ
			//--------------------------------------------------------------------------------------
			enum class SelectType {
				Priority,	//優先度
				Sequence,	//シーケンス
				Random,		//ランダム
			};

			//--------------------------------------------------------------------------------------
			/// ビヘイビアセレクター
			//--------------------------------------------------------------------------------------
			class Selecter : public NodeBase
			{
			public:
				using SelecterType = SelectType;

			private:
				std::weak_ptr<I_Node> m_currentNode;					//現在使用中のノード

				SelectType m_selectType;								//セレクトタイプ

				std::weak_ptr<I_Edge> m_fromEdge;						//手前のエッジ
				std::vector<std::weak_ptr<I_Edge>> m_transitionEdges;	//遷移先のエッジ配列

			public:
				Selecter();
				Selecter(const SelectType selectType);

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;

			private:

				/// <summary>
				/// 一番優先度の高いノードを返す。
				/// </summary>
				/// <returns>一番優先度が高いノード</returns>
				std::shared_ptr<I_Node> SearchFirstPriorityNode() const;

				/// <summary>
				/// ランダムに遷移先を選ぶ。
				/// </summary>
				/// <returns>ランダムな遷移先</returns>
				std::shared_ptr<I_Node> SearchRandomNode() const;

				/// <summary>
				/// シーケンサとしての遷移先を選ぶ
				/// </summary>
				/// <returns>シーケンサとしての遷移先</returns>
				std::shared_ptr<I_Node> SearchSequenceNode() const;

				/// <summary>
				/// 現在使用中のノードを切り替える。
				/// </summary>
				void ChangeCurrentNode(const std::shared_ptr<I_Node>& node);

			public:
				/// <summary>
				/// 更新するノードを検索
				/// </summary>
				std::shared_ptr<I_Node> SearchCurrentNode() const;

				//--------------------------------------------------------------------------------------
				/// アクセッサ
				//--------------------------------------------------------------------------------------

				void SetSelectType(const SelectType type) { m_selectType = type; }

				SelectType GetSelectType() const { return m_selectType; }

				/// <summary>
				/// 手前エッジの設定
				/// </summary>
				void SetFromEdge(const std::shared_ptr<I_Edge>& fromEdge) noexcept;

				/// <summary>
				/// 手前エッジの取得
				/// </summary>
				_NODISCARD std::shared_ptr<I_Edge> GetFromEdge() const noexcept;

				/// <summary>
				/// 遷移先エッジの追加
				/// </summary>
				void AddTransitionEdge(const std::shared_ptr<I_Edge>& edge);

				/// <summary>
				/// 遷移先のノードが存在するかどうか
				/// </summary>
				bool IsEmptyTransitionNodes() const;

				/// <summary>
				/// カレントノードが存在するかどうか
				/// </summary>
				bool HasCurrentNode() const noexcept { return !m_currentNode.expired(); }

				/// <summary>
				/// 現在のノードを設定
				/// </summary>
				/// <param name="node">設定するノード</param>
				void SetCurrentNode(const std::shared_ptr<I_Node>& node) noexcept;

				/// <summary>
				/// 現在使用中のノードを返す
				/// </summary>
				_NODISCARD std::shared_ptr<I_Node> GetCurrentNode() const noexcept;

			};

		}
	}
}