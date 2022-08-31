
/*!
@file TimeHelper.h
@brief TimeHelperクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	計測時間タイプ
		//--------------------------------------------------------------------------------------
		enum class DeltaType {
			Normal,  //通常
			Real,    //リアルタイム
		};

		//--------------------------------------------------------------------------------------
		///	時間管理ヘルパー
		//--------------------------------------------------------------------------------------
		class TimeHelper 
		{
		public:
			/// <summary>
			/// 経過時間の取得
			/// </summary>
			/// <param name="type">計測時間タイプ</param>
			/// <returns>経過時間</returns>
			static float GetElapsedTime(const DeltaType& type);

		};

	}
}

//endbasecross