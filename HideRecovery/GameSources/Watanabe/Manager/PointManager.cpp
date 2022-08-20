#include "stdafx.h"
#include "PointManager.h"

namespace basecross {
	// インスタンスの初期化
	unique_ptr<PointManager, PointManager::Deleter> PointManager::m_instance = nullptr;

	int PointManager::GetPoint(Team team) {
	}
	void PointManager::AddPoint() {
	}
}