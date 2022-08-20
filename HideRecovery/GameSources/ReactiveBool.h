/*!
@file ReactiveBool.h
@brief ReactiveBool
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "MaruAction.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	リアクティブbool
		//--------------------------------------------------------------------------------------
		class ReactiveBool
		{
		private:
			bool value;							//本体データ
			maru::Action<void()> m_trueAction;  //trueに切り替わった時に呼び出すイベント
			maru::Action<void()> m_falseAction; //falseに切り替わった時に呼び出すイベント

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			ReactiveBool();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="b">boolの初期値</param>
			ReactiveBool(const bool b);

			virtual ~ReactiveBool() = default;

		public:
			//--------------------------------------------------------------------------------------
			///	アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 切り替えたときに呼び出すイベントを追加
			/// </summary>
			/// <param name="b">どちらに切り替えたときに呼び出すか</param>
			/// <param name="function">呼び出すイベント</param>
			void AddFunction(const bool b, const std::function<void()>& function);

			/// <summary>
			/// 本体データの取得
			/// </summary>
			/// <returns>本体データ</returns>
			bool GetValue() const;

			//--------------------------------------------------------------------------------------
			///	オペレータ
			//--------------------------------------------------------------------------------------

			bool operator = (const bool b);

			operator bool() const;

		};

	}
}

//endbasecross