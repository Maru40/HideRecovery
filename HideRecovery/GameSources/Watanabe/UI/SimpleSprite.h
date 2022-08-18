#pragma once
#include "stdafx.h"

namespace basecross {
	class SimpleSprite :public GameObject {
	public:
		enum class Type {
			Texture,
			SpriteData
		};
	private:
		weak_ptr<PCTSpriteDraw> m_spriteDraw;
		Type m_type;
		wstring m_key;
	public:
		SimpleSprite(const shared_ptr<Stage>& stage, Type type, const wstring& key);

		void OnCreate()override;
		shared_ptr<PCTSpriteDraw> GetDrawComponent();
	};
}
