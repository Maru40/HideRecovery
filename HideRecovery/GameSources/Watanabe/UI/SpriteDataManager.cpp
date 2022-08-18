#include "stdafx.h"
#include "SpriteDataManager.h"

namespace basecross {
	// インスタンスの初期化
	unique_ptr<SpriteDataManager, SpriteDataManager::Deleter> SpriteDataManager::m_instance = nullptr;

	SpriteData SpriteDataManager::GetSpriteData(const wstring name) {
	}
}