#include "stdafx.h"
#include "SimpleSprite.h"

namespace basecross {
	SimpleSprite::SimpleSprite(const shared_ptr<Stage>& stage, Type type, const wstring& key)
		:GameObject(stage), m_type(type), m_key(key)
	{}

	void SimpleSprite::SetupTexture() {
	}
	void SimpleSprite::SetupSpriteData() {
	}

	void SimpleSprite::OnCreate() {
		switch (m_type)
		{
		case SimpleSprite::Type::Texture:
			SetupTexture();
			break;
		case SimpleSprite::Type::SpriteData:
			SetupSpriteData();
			break;
		default:
			break;
		}
	}

	shared_ptr<PCTSpriteDraw> SimpleSprite::GetDrawComponent() {
		return m_spriteDraw.lock();
	}
}