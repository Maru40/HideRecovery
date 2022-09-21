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

		}
	}
}