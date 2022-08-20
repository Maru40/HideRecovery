#include "stdafx.h"
#include "SimpleSprite.h"
#include "../Utility/Utility.h"
#include "../Manager/SpriteDataManager.h"

namespace basecross {
	SimpleSprite::SimpleSprite(const shared_ptr<Stage>& stage, Type type, const wstring& key)
		:GameObject(stage), m_type(type), m_key(key)
	{}

	void SimpleSprite::OnCreate() {
		vector<VertexPositionColorTexture> vertices;
		sdm::SpriteData data;
		switch (m_type)
		{
		case SimpleSprite::Type::Texture:
			data.useTextureKey = m_key;
			data.origin = Vec2(0.0f);
			data.size = Utility::GetTextureSize(m_key);
			break;
		case SimpleSprite::Type::SpriteData:
			data = SpriteDataManager::GetInstance()->GetSpriteData(m_key);
			break;
		}
		Vec2 size = data.size;
		vector<Vec2> uv01, uv = {
			data.origin + Vec2(0.0f,0.0f),
			data.origin + Vec2(size.x,0.0f),
			data.origin + Vec2(0.0f,size.y),
			data.origin + size
		};
		Utility::ConvertToUVCoordinates(uv, data.useTextureKey, uv01);
		auto halfSize = size / 2.0f;
		Col4 color(1);
		vertices = {
			{Vec3(-halfSize.x, +halfSize.y, 0.0f), color, uv01[0]},
			{Vec3(+halfSize.x, +halfSize.y, 0.0f), color, uv01[1]},
			{Vec3(-halfSize.x, -halfSize.y, 0.0f), color, uv01[2]},
			{Vec3(+halfSize.x, -halfSize.y, 0.0f), color, uv01[3]}
		};
		vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3
		};
		m_spriteDraw = AddComponent<PCTSpriteDraw>(vertices, indices);
		m_spriteDraw.lock()->SetTextureResource(data.useTextureKey);

		SetAlphaActive(true);
	}

	shared_ptr<PCTSpriteDraw> SimpleSprite::GetDrawComponent() {
		return m_spriteDraw.lock();
	}
}