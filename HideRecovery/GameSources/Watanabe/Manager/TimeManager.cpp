#include "stdafx.h"
#include "TimeManager.h"

namespace basecross {
	// インスタンスの初期化
	unique_ptr<TimeManager, TimeManager::Deleter> TimeManager::m_instance = nullptr;

	TimeManager::TimeManager()
		:m_timer(300)
	{}

	void TimeManager::UpdateTime() {
		m_timer.Count();

		for (auto& eventData : m_eventDataMap) {
			if (eventData.first > m_timer.GetLeftTime() && !eventData.second.IsExecuted) {
				for (auto func : eventData.second.Functions) {
					if (func)
						func();
				}
				eventData.second.IsExecuted = true;
			}
		}
	}
	TimeCounter TimeManager::GetTimer() {
		return m_timer;
	}

	void TimeManager::AddEvent(float time, function<void()> func) {
		if (m_eventDataMap.count(time) == 0) {
			vector<function<void()>> funcs;
			funcs.push_back(func);

			EventData eventData;
			eventData.IsExecuted = false;
			eventData.Functions = funcs;

			m_eventDataMap[time] = eventData;
			return;
		}
		m_eventDataMap[time].Functions.push_back(func);
	}
}