/*!
@file I_Node.h
@brief I_Nodeなど
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
			class I_Decorator;

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

		}
	}
}