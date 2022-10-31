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