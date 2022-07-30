
/*!
@file MaruAction.h
@brief MaruActionクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	アクションクラス
		//--------------------------------------------------------------------------------------
		template<class T>
		class Action
		{
			std::vector<std::function<T>> m_functions; //イベント配列

		public:
			/// <summary>
			/// イベントを追加する。
			/// </summary>
			/// <param name="function">追加したいイベント</param>
			void AddFunction(const std::function<T>& function) {
				if (function) {
					m_functions.push_back(function);
				}
			}

			/// <summary>
			/// 登録されたイベントを全て呼び出す。
			/// </summary>
			/// <param name="params">引数パラメータパック</param>
			template<class... Ts>
			void Invoke(Ts&&... params) {
				for (auto& function : m_functions) {
					function(params...);
				}
			}

			/// <summary>
			/// 登録したイベントを全て消去
			/// </summary>
			void Clear() {
				m_functions.clear();
			}

			//--------------------------------------------------------------------------------------
			///	オペレータ
			//--------------------------------------------------------------------------------------

			Action operator +  (const std::function<T>& function) {
				m_functions.push_back(function);
				return *this;
			}

			Action operator += (const std::function<T>& function) {
				return *this + function;
			}

		};

	}
}