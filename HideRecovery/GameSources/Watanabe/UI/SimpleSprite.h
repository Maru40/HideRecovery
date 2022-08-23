#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"

namespace basecross {
	class SimpleSprite :public UIObjectBase {
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
		SimpleSprite(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;
		shared_ptr<PCTSpriteDraw> GetDrawComponent();
	};
}
