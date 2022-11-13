/*!
@file I_Decorator.h
@brief I_Decoratorなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

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
				/// 生成時に一度だけ呼ぶ処理
				/// </summary>
				virtual void OnCreate() = 0;

				/// <summary>
				/// ノード開始時に呼び出す処理。
				/// </summary>
				virtual void OnStart() = 0;

				/// <summary>
				/// 遷移条件確認前の準備(CanTransitionを呼ぶ前に呼ぶ処理)
				/// </summary>
				virtual void ReserveCanTransition() = 0;

				/// <summary>
				/// 遷移できるかどうか
				/// </summary>
				/// <returns>遷移できるならtrue</returns>
				virtual bool CanTransition() const = 0;

				/// <summary>
				/// アップデートが可能かどうか
				/// </summary>
				/// <returns>アップデートが可能ならtrue</returns>
				virtual bool CanUpdate() = 0;
			};

			//--------------------------------------------------------------------------------------
			/// デコレータの基底クラス
			//--------------------------------------------------------------------------------------
			template<class OwnerType>
			class DecoratorBase : public I_Decorator
			{
				std::weak_ptr<OwnerType> m_owner;	//オーナー

			public:
				DecoratorBase(const std::shared_ptr<OwnerType>& owner) :
					m_owner(owner)
				{}

				virtual ~DecoratorBase() = default;

				virtual void OnCreate() override {}
				virtual void ReserveCanTransition() override {};
				virtual void OnStart() override {}

				//--------------------------------------------------------------------------------------
				/// アクセッサ
				//--------------------------------------------------------------------------------------

				_NODISCARD std::shared_ptr<OwnerType> GetOwner() const noexcept {
					return m_owner.lock();
				}
			};

		}
	}
}