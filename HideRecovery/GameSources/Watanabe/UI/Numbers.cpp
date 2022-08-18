#include "stdafx.h"
#include "Numbers.h"
#include "../Utility/Utility.h"
#include "SpriteDataManager.h"

namespace basecross {
	NumberSprite::NumberSprite(const shared_ptr<Stage>& stage)
		:GameObject(stage), m_number(0)
	{}

	void NumberSprite::OnCreate() {
		auto data = SpriteDataManager::GetInstance()->GetSpriteData(L"Numbers");

		Vec2 size = data.size;
		float fnumber = static_cast<float>(m_number);
		vector<Vec2> uv01, uv = {
			Vec2((fnumber + 0) * size.x,0.0f),
			Vec2((fnumber + 1) * size.x,0.0f),
			Vec2((fnumber + 0) * size.x,size.y),
			Vec2((fnumber + 1) * size.x,size.y)
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
		std::vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3
		};
		m_spriteDraw = AddComponent<PCTSpriteDraw>(vertices, indices);
		m_spriteDraw.lock()->SetTextureResource(data.useTextureKey);

		SetAlphaActive(true);
	}

	void NumberSprite::SetValue(int number) {
		auto data = SpriteDataManager::GetInstance()->GetSpriteData(L"Numbers");
		m_number = number;
		Vec2 size = data.size;
		float fnumber = static_cast<float>(number);
		vector<Vec2> uv01, uv = {
			Vec2((fnumber + 0) * size.x,0.0f),
			Vec2((fnumber + 1) * size.x,0.0f),
			Vec2((fnumber + 0) * size.x,size.y),
			Vec2((fnumber + 1) * size.x,size.y)
		};
		Utility::ConvertToUVCoordinates(uv, data.useTextureKey, uv01);
		auto halfSize = size / 2.0f;
		Col4 color(1);
		for (int i = 0; i < vertices.size(); i++) {
			vertices[i].textureCoordinate = uv01[i];
		}

		m_spriteDraw.lock()->UpdateVertices(vertices);
	}

	Numbers::Numbers(const shared_ptr<Stage>& stage)
		:GameObject(stage)
	{}

	void Numbers::OnCreate() {
	}
	void Numbers::SetNumber(int num) {
	}
}