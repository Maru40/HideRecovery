#include "stdafx.h"
#include "ScoreManager.h"

namespace basecross {
	// インスタンスの初期化
	unique_ptr<ScoreManager, ScoreManager::Deleter> ScoreManager::m_instance = nullptr;
}