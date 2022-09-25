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
			};

		}
	}
}