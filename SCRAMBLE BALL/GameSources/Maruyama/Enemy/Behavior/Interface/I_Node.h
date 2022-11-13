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
			/// ビヘイビアのノードステート
			//--------------------------------------------------------------------------------------
			enum class BehaviorState {
				Inactive,	//非アクティブ(待機状態)
				Success,	//成功
				Failure,	//失敗
				Running,	//実行中
				Completed,	//完了
			};

			//--------------------------------------------------------------------------------------
			/// ビヘイビアノードのインターフェース
			//--------------------------------------------------------------------------------------
			class I_Node {
			public:
				virtual ~I_Node() = default;

				virtual void OnDecoratorStart() = 0;
				virtual void OnDecoratorExit() = 0;

				virtual void OnStart() = 0;
				virtual bool OnUpdate() = 0;
				virtual void OnExit() = 0;

				/// <summary>
				/// 処理が中断されたときに呼び出す処理
				/// </summary>
				virtual void OnAbort() {}

				virtual void SetIsActive(const bool isActive) noexcept = 0;

				virtual bool IsActive() const noexcept = 0;

				/// <summary>
				/// インデックスのセット
				/// </summary>
				virtual void SetIndex(const int index) noexcept = 0;

				/// <summary>
				/// インデックスの取得
				/// </summary>
				/// <returns>インデックス</returns>
				virtual int GetIndex() const noexcept = 0;

				/// <summary>
				/// タイプの設定
				/// </summary>
				template<class EnumType>
				void SetType(const EnumType type) { SetIndex(static_cast<int>(type)); }

				/// <summary>
				/// タイプの取得
				/// </summary>
				template<class EnumType>
				EnumType GetType() const { return static_cast<EnumType>(GetIndex()); }

				/// <summary>
				/// ステートの設定
				/// </summary>
				/// <param name="state">ステート</param>
				virtual void SetState(const BehaviorState state) noexcept = 0;

				/// <summary>
				/// ビヘイビアステートの取得
				/// </summary>
				/// <returns>ビヘイビアステート</returns>
				virtual BehaviorState GetState() const noexcept = 0;

				/// <summary>
				/// 引数で渡したステートかどうか判断
				/// </summary>
				/// <param name="state">確認したいステート</param>
				/// <returns>渡したステート同じならtrue</returns>
				virtual bool IsState(const BehaviorState state) const noexcept = 0;

				/// <summary>
				/// 遷移できるかどうか
				/// </summary>
				/// <returns>遷移できるならtrue</returns>
				virtual bool CanTransition() const = 0;

				/// <summary>
				/// 更新処理を続けらるかどうか
				/// </summary>
				/// <returns>続けらるならtrue</returns>
				virtual bool CanUpdate() const = 0;

				/// <summary>
				/// デコレータの追加
				/// </summary>
				virtual void AddDecorator(const std::shared_ptr<I_Decorator>& decorator) = 0;

				/// <summary>
				/// デコレータ配列の取得
				/// </summary>
				/// <returns>デコレータ配列</returns>
				virtual std::vector<std::shared_ptr<I_Decorator>> GetDecorators() const = 0;

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
				bool m_isActive = true;									//アクティブ状態かどうか
				int m_index = 0;										//ノードインデックス
				BehaviorState m_state = BehaviorState::Inactive;		//ビヘイビアステート
				std::vector<std::shared_ptr<I_Decorator>> m_decorators;	//デコレータ配列

			public:
				virtual ~NodeBase() = default;

				virtual void OnDecoratorStart() override;
				virtual void OnDecoratorExit() override;

				virtual void SetIsActive(const bool isActive) noexcept override { m_isActive = isActive; };

				virtual bool IsActive() const noexcept override { return m_isActive; };

				void SetIndex(const int index) noexcept override { m_index = index; }

				int GetIndex() const noexcept override { return m_index; }

				void SetState(const BehaviorState state) noexcept override { m_state = state; }

				BehaviorState GetState() const noexcept override { return m_state; }

				bool IsState(const BehaviorState state) const noexcept override { return m_state == state; }

				bool CanTransition() const override;

				bool CanUpdate() const override;

				void AddDecorator(const std::shared_ptr<I_Decorator>& decorator) override;

				std::vector<std::shared_ptr<I_Decorator>> GetDecorators() const override;

				bool IsDecoratorEmpty() const override;
			};

		}
	}
}