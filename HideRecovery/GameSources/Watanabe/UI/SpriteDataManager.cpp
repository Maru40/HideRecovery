#include "stdafx.h"
#include "SpriteDataManager.h"
#include "../Utility/CSVLoad.h"
#include "../Utility/DataExtracter.h"

namespace basecross {
	// インスタンスの初期化
	unique_ptr<SpriteDataManager, SpriteDataManager::Deleter> SpriteDataManager::m_instance = nullptr;
	const wstring SpriteDataManager::LoadCSVFileKey = L"SpriteData";

	wt::SpriteData SpriteDataManager::GetSpriteData(const wstring name) {
		// データがなければ作る
		if (m_spriteDataMap.count(name) == 0) {
			// ファイルを読み込みnameに該当するデータを検索する
			auto spriteDatas = CSVLoad::GetInstance()->GetData(LoadCSVFileKey);
			auto targetIndex = CSVHelper::SearchIndexDataTarget(spriteDatas, name, 0);
			if (targetIndex == -1) {
				throw BaseException(
					L"SpriteDataに目的のデータがありません。",
					L"name : " + name,
					L"SpriteDataManager::GetSpriteData()"
				);
			}
			auto tokens = DataExtracter::DelimitData(spriteDatas[targetIndex]);

			wt::SpriteData data = {};
			data.useTextureKey = tokens[1];
			data.origin = DataExtracter::Vector2DataExtraction(tokens, 2);
			data.size = DataExtracter::Vector2DataExtraction(tokens, 4);
			m_spriteDataMap[name] = data;
		}
		return m_spriteDataMap[name];
	}
}