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
	}
	TimeCounter TimeManager::GetTimer() {
		return m_timer;
	}
}