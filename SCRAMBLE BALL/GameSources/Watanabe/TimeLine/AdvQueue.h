/*!
@file   AdvQueue.h
@brief  queueの拡張コンテナ
*/

#pragma once
#include "stdafx.h"
#include <queue>

namespace basecross {
	/**
	 * @brief queueの拡張コンテナクラス
	 * @tparam コンテナに格納するデータ型
	 */
	template<class T>
	class AdvQueue :public queue<T, list<T>> {
	public:
		AdvQueue() = default;
		explicit AdvQueue(const list<T>& _Cont) : queue(_Cont) {}

		/**
		 * @brief コンテナのクリア
		 */
		void clear() {
			c.clear();
		}
	};
}
