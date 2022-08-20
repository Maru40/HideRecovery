#include "stdafx.h"
#include "TimeManager.h"

namespace basecross {
	// インスタンスの初期化
	unique_ptr<TimeManager, TimeManager::Deleter> TimeManager::m_instance = nullptr;
}