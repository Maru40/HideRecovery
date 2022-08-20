#pragma once
#include "stdafx.h"
#include "../Utility/BaseSingleton.h"
#include "../Utility/TimeCounter.h"

namespace basecross {
	class TimeManager :public BaseSingleton<TimeManager> {
		TimeCounter m_timer;
	public:
		/// <summary>
		/// タイマーのアップデート
		/// </summary>
		void UpdateTime();
		/// <summary>
		/// タイマーを取得
		/// </summary>
		/// <returns>タイマー</returns>
		TimeCounter GetTimer();
	private:
		TimeManager();
		~TimeManager() {}
		friend class BaseSingleton<TimeManager>;
	};
}
