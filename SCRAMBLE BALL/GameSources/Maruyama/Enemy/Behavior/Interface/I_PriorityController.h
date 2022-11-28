/*!
@file I_PriorityController.h
@brief I_PriorityControllerなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			//--------------------------------------------------------------------------------------
			/// 優先度管理のインターフェース
			//--------------------------------------------------------------------------------------
			class I_PriorityController {
			public:
				virtual ~I_PriorityController() = default;

				/// <summary>
				/// 優先度の計算をする
				/// </summary>
				/// <returns>計算した優先度</returns>
				virtual float CalculatePriority() = 0;
			};

			//--------------------------------------------------------------------------------------
			/// 優先度管理の基底クラス
			//--------------------------------------------------------------------------------------
			template<class OwnerType>
			class PriorityControllerBase : public I_PriorityController {
				std::weak_ptr<OwnerType> m_owner;	//所有者

			public:
				PriorityControllerBase(const std::shared_ptr<OwnerType>& owner):
					m_owner(owner)
				{}

				virtual ~PriorityControllerBase() = default;

				/// <summary>
				/// 所有者の取得
				/// </summary>
				/// <returns>所有者</returns>
				std::shared_ptr<OwnerType> GetOwner() const noexcept { return m_owner.lock(); }
			};

		}
	}
}