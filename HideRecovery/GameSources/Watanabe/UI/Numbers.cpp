#include "stdafx.h"
#include "Numbers.h"
#include "../Utility/Utility.h"

namespace basecross {
	NumberSprite::NumberSprite(const shared_ptr<Stage>& stage)
		:GameObject(stage), m_number(0)
	{}

	void NumberSprite::OnCreate() {
		const wstring TextureKey = L"Numbers_TX";

		float fontwidth = 102.0f;
		float fnumber = static_cast<float>(m_number);
		vector<Vec2> uv01, uv = {
			Vec2((fnumber + 0) * fontwidth,0.0f),
			Vec2((fnumber + 1) * fontwidth,0.0f),
			Vec2((fnumber + 0) * fontwidth,64.0f),
			Vec2((fnumber + 1) * fontwidth,64.0f)
		};
		Utility::ConvertToUVCoordinates(uv, TextureKey, uv01);
		Col4 color(1);
		vertices = {
			{Vec3(0.0f     ,    0.0f, 0.0f), color, uv01[0]},
			{Vec3(fontwidth,    0.0f, 0.0f), color, uv01[1]},
			{Vec3(0.0f     , -100.0f, 0.0f), color, uv01[2]},
			{Vec3(fontwidth, -100.0f, 0.0f), color, uv01[3]}
		};
		std::vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3
		};
		auto drawComp = AddComponent<PCTSpriteDraw>(vertices, indices);
		drawComp->SetTextureResource(TextureKey);

		SetAlphaActive(true);
	}

	void NumberSprite::SetValue(int number) {
		float fontwidth = 102.0f;
		float fnumber = static_cast<float>(number);
		vector<Vec2> uv01, uv = {
			Vec2((fnumber + 0) * fontwidth,0.0f),
			Vec2((fnumber + 1) * fontwidth,0.0f),
			Vec2((fnumber + 0) * fontwidth,64.0f),
			Vec2((fnumber + 1) * fontwidth,64.0f)
		};
		Utility::ConvertToUVCoordinates(uv, L"Numbers_TX", uv01);
		Col4 color(1);
		vertices = {
			{Vec3(0.0f     ,    0.0f, 0.0f), color, uv01[0]},
			{Vec3(fontwidth,    0.0f, 0.0f), color, uv01[1]},
			{Vec3(0.0f     , -100.0f, 0.0f), color, uv01[2]},
			{Vec3(fontwidth, -100.0f, 0.0f), color, uv01[3]}
		};

		GetComponent<PCTSpriteDraw>()->UpdateVertices(vertices);
	}

	Numbers::Numbers(const shared_ptr<Stage>& stage)
		:GameObject(stage)
	{}

	void Numbers::OnCreate() {
	}
	void Numbers::SetNumber(int num) {
	}
}