#pragma once
#include "stdafx.h"
#include "../Utility/BaseSingleton.h"

namespace basecross {
	struct SpriteData {
		// 使うテクスチャのキー
		wstring useTextureKey;
		// 原点
		Vec2 origin;
		// サイズ
		Vec2 size;
	};

	class SpriteDataManager :public BaseSingleton<SpriteDataManager> {
		map<wstring, SpriteData> m_spriteDataMap;
	public:
		/// <summary>
		/// 任意のSpriteDataを取得する
		/// </summary>
		/// <param name="name">識別用の名前</param>
		/// <returns>SpriteData</returns>
		SpriteData GetSpriteData(const wstring name);
	private:
		SpriteDataManager() {}
		~SpriteDataManager() {}
		friend class BaseSingleton<SpriteDataManager>;
	};
}
