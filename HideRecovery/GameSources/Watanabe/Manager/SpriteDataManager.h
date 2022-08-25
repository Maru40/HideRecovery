#pragma once
#include "stdafx.h"
#include "../Utility/BaseSingleton.h"

namespace basecross {
	namespace sdm {
		struct SpriteData {
			// 使うテクスチャのキー
			wstring useTextureKey;
			// 原点
			Vec2 origin;
			// サイズ
			Vec2 size;

			SpriteData()
				:useTextureKey(L""), origin(Vec2(0.0f)), size(Vec2(0.0f))
			{}
		};
	}

	class SpriteDataManager :public BaseSingleton<SpriteDataManager> {
		map<wstring, sdm::SpriteData> m_spriteDataMap;
		static const wstring LoadCSVFileKey;
	public:
		/// <summary>
		/// 任意のSpriteDataを取得する
		/// </summary>
		/// <param name="name">識別用の名前</param>
		/// <returns>SpriteData</returns>
		sdm::SpriteData GetSpriteData(const wstring& name);
	private:
		SpriteDataManager() {}
		~SpriteDataManager() {}
		friend class BaseSingleton<SpriteDataManager>;
	};
}
