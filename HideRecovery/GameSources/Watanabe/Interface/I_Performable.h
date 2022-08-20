/*!
@file   I_Performable.h
@brief  一連の流れの演出を操作するインターフェイス
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class I_Performable {
	public:
		/// <summary>
		/// 演出開始
		/// </summary>
		virtual void Start() = 0;

		/// <summary>
		/// 演出のリセット
		/// </summary>
		virtual void Reset() = 0;

		/// <summary>
		/// 有効の切り替え
		/// </summary>
		/// <param name="flg">trueで有効</param>
		virtual void SetActive(bool flg) = 0;
	};
}
