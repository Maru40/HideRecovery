/*!
@file ReactiveProperty.h
@brief ReactiveProperty
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "MaruAction.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	リアクティブプロパティ
	//--------------------------------------------------------------------------------------
	template<class T>
	class ReactiveProperty 
	{
	private:
		T value;						//本体データ

		maru::Action<void()> m_action;	//呼び出すイベント群

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="ptr">本体データ</param>
		ReactiveProperty(const T& value):
			value(value)
		{}

		virtual ~ReactiveProperty() {};

		/// <summary>
		/// 切り替えた時に呼びたい処理
		/// </summary>
		/// <param name="whereAction">条件式</param>
		/// <param name="action">呼び出したい処理</param>
		void AddSubscribe(const std::function<bool()>& whereAction, const std::function<void()>& action) {
			m_action.AddFunction(
				[&, whereAction, action]() {
					if (whereAction()) { action(); };
				}
			);
		}

		/// <summary>
		/// 切り替えた時に呼びたい処理
		/// </summary>
		/// <param name="whereAction">条件式</param>
		/// <param name="action">呼び出したい処理</param>
		void AddSubscribe(const maru::Action<bool()>& whereAction, const maru::Action<void()>& action) {
			m_action.AddFunction(
				[&, whereAction, action]() {
					if (whereAction.Invoke()) { action.Invoke(); };
				}
			);
		}

		void SetValue(const T& value) { this->value = value; }

		/// <summary>
		/// 本体データを取得
		/// </summary>
		/// <returns>本体データ</returns>
		T GetValue() const noexcept { return value; }

		/// <summary>
		/// 本体データの生ポインタを取得
		/// </summary>
		/// <returns>本体データの生ポインタ</returns>
		T* get() const { return &value; }

		//--------------------------------------------------------------------------------------
		///	オペレータ
		//--------------------------------------------------------------------------------------

		T operator = (const T t) {
			//if (&GetValue() != &t) { //違うのなら
			//	m_action.Invoke();
			//}

			this->value = t;
			return this->value;
		}

		bool operator == (const T& other) const {
			return &GetValue() == &other;
		}

		bool operator != (const T& other) const {
			return !(&GetValue() == &other);
		}

	};
}