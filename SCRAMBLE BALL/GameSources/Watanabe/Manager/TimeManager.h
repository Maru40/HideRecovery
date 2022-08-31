#pragma once
#include "stdafx.h"
#include "../Utility/BaseSingleton.h"
#include "../Utility/TimeCounter.h"

namespace basecross {
	class TimeManager :public BaseSingleton<TimeManager> {
		struct EventData {
			bool IsExecuted;
			vector<function<void()>> Functions;
		};
		TimeCounter m_timer;
		map<float, EventData> m_eventDataMap;
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

		/// <summary>
		/// イベントをセット
		/// </summary>
		/// <param name="time">発動時間</param>
		/// <param name="func">実行する関数</param>
		void AddEvent(float time, function<void()> func);
	private:
		TimeManager();
		~TimeManager() {}
		friend class BaseSingleton<TimeManager>;
	};
}
