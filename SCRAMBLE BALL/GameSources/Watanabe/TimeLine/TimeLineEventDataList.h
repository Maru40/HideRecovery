/*!
@file   TimeLineEventDataList.h
@brief  タイムライン上に設定されるイベント管理クラス
*/

#pragma once
#include "stdafx.h"
#include "AdvQueue.h"

namespace basecross {
	namespace timeline {
		using Func = function<void()>;

		struct TimeLineEventData {
			// 実行する時間
			float ExecutingTime;
			// イベント
			Func Function;

			TimeLineEventData(float time, const Func& func)
				:ExecutingTime(time), Function(func)
			{}
		};

		class TimeLineEventDataList {
			// イベントリスト
			list<shared_ptr<TimeLineEventData>> m_eventList;
			// イベントキュー
			AdvQueue<shared_ptr<TimeLineEventData>> m_eventQueue;
		public:
			TimeLineEventDataList() {}

			void Initialize();
			void Update(float delta);
			void AddEvent(float executingTime, const Func& func);
			void ClearEvent();
		};
	}
}
