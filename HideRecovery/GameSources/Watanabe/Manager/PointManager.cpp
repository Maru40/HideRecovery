#include "stdafx.h"
#include "PointManager.h"

namespace basecross {
	// インスタンスの初期化
	unique_ptr<PointManager, PointManager::Deleter> PointManager::m_instance = nullptr;

	PointManager::PointManager() {
		// 点数の初期化
		m_teamScoreMap[Team::East] = 0;
		m_teamScoreMap[Team::West] = 0;
	}

	int PointManager::GetPoint(Team team) {
		return m_teamScoreMap[team];
	}
	void PointManager::AddPoint(Team team) {
		m_teamScoreMap[team]++;
	}
}