/*!
@file FutureData.h
@brief FutureDataなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include <future>

namespace basecross {

	template<class R>
	struct FutureData {
		mutable std::atomic<bool> m_isRunning;	//更新中かどうか
		std::future<R> m_future;				//フューチャーデータ

		FutureData() :
			m_isRunning(true)
		{}

		/// <summary>
		/// フューチャーデータの引き渡し。
		/// </summary>
		/// <param name="other">引き渡しデータ</param>
		void MoveFuture(std::future<R>& other) {
			m_future = std::move(other);
		}
	};

}