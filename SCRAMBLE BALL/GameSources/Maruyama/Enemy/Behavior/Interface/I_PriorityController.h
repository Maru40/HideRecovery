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
				/// 優先度の設定
				/// </summary>
				/// <param name="priority">優先度</param>
				virtual void SetPriority(const float priority) = 0;

				/// <summary>
				/// 優先度の取得
				/// </summary>
				/// <returns>優先度</returns>
				virtual float GetPriority() const = 0;

				/// <summary>
				/// 優先度の計算をする
				/// </summary>
				/// <returns>計算した優先度</returns>
				virtual float CalculatePriority() = 0;
			};

			//--------------------------------------------------------------------------------------
			/// 優先度管理の基底クラス
			//--------------------------------------------------------------------------------------
			class PriorityControllerBase : public I_PriorityController {
				float m_priority = 0;

			public:
				PriorityControllerBase();
				PriorityControllerBase(const float priority);

				virtual ~PriorityControllerBase() = default;

				void SetPriority(const float priority) override { m_priority = priority; };

				float GetPriority() const override { return m_priority; }

				float CalculatePriority() override { return m_priority; }	//本来は純粋仮想関数にする予定
			};

		}
	}
}