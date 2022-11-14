/*!
@file   TimeLineEventDataList.cpp
@brief  タイムライン上に設定されるイベント管理クラス実体
*/

#include "stdafx.h"
#include "TimeLineEventDataList.h"
#include "../DebugClass/Debug.h"

namespace basecross {
	namespace timeline {
		void TimeLineEventDataList::Initialize() {
			// 実行順にソート
			m_eventList.sort(
				[](shared_ptr<TimeLineEventData> left, shared_ptr<TimeLineEventData> right) {
					return left->ExecutingTime < right->ExecutingTime;
				}
			);
			m_eventQueue = AdvQueue<shared_ptr<TimeLineEventData>>(m_eventList);
		}

		void TimeLineEventDataList::Update(float delta) {
			if (m_eventQueue.empty())
				return;

			auto currentEventData = m_eventQueue.front();
			// 指定した時間が経過した
			if (delta > currentEventData->ExecutingTime) {
				// 関数が有効であれば実行
				if (auto func = currentEventData->Function)
					func();

				// 先頭を削除
				m_eventQueue.pop();
				// 同じ時間帯のイベントがあればなくなるまでループ
				while (delta > m_eventQueue.front()->ExecutingTime) {
					if (auto func = m_eventQueue.front()->Function)
						func();
					m_eventQueue.pop();

					// このタイミングで空なら終了
					if (m_eventQueue.empty())
						break;
				}
			}
		}
		void TimeLineEventDataList::AddEvent(float executingTime, const Func& func) {
			m_eventList.push_back(
				make_shared<TimeLineEventData>(executingTime, func)
			);
		}

		void TimeLineEventDataList::ClearEvent() {
			m_eventList.clear();
		}
	}
}