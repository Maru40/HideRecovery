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
		std::weak_ptr<T> value;        //本体データ

		maru::Action<void()> m_action; //呼び出すイベント群

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="ptr">本体データのポインタ</param>
		ReactiveProperty(const std::shared_ptr<T>& ptr) {
			value = ptr;
		}

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

		/// <summary>
		/// 本体データのポインタを取得
		/// </summary>
		/// <returns>本体データのポインタ</returns>
		std::shared_ptr<T> GetValue() const {
			return value.lock();
		}

		/// <summary>
		/// 本体データのポインタを取得
		/// </summary>
		/// <returns>本体データのポインタ</returns>
		std::shared_ptr<T> GetShared() const {
			return value.lock();
		}

		/// <summary>
		/// 本体データの生ポインタを取得
		/// </summary>
		/// <returns>本体データの生ポインタ</returns>
		T* get() const {
			return value.lock().get();
		}

		//--------------------------------------------------------------------------------------
		///	オペレータ
		//--------------------------------------------------------------------------------------

		std::shared_ptr<T> operator = (const std::shared_ptr<T> t) {
			if (this->value.lock() != t) { //違うのなら
				m_action.Invoke();
			}

			this->value = t;
			return this->value.lock();
		}

		/// <summary>
		/// 本体データのポインタを取得
		/// </summary>
		/// <returns>本体データのポインタ</returns>
		std::shared_ptr<T> operator -> () const {
			return value.lock();
		}

		bool operator == (const std::shared_ptr<T>& other) const {
			return get() == other.get();
		}

		bool operator == (const ex_weak_ptr<T>& other) const {
			return get() == other.get();
		}

		bool operator != (const std::shared_ptr<T>& other) const {
			return !(get() == other.get());
		}
		bool operator != (const ex_weak_ptr<T>& other) const {
			return !(get() == other.get());
		}

		operator bool() const {
			return !value.expired();
		}

	};
}