
#include "stdafx.h"
#include "Number.h"

namespace basecross {
	void Number::OnCreate()
	{
		float fNumber = static_cast<float>(number);
		Col4 color(1.0f, 1.0f, 1.0f, 1.0f);
		vertices = {
			{Vec3(0.0f,    0.0f,0.0f),color,Vec2((fNumber + 0) * 90.0f / 995.0f, 0.0f)},
			{Vec3(50.0f,   0.0f,0.0f),color,Vec2((fNumber + 1) * 90.0f / 995.0f, 0.0f)},
			{Vec3(0.0f, -100.0f,0.0f),color,Vec2((fNumber + 0) * 90.0f / 995.0f, 1.0f)},
			{Vec3(50.0f,-100.0f,0.0f),color,Vec2((fNumber + 1) * 90.0f / 995.0f, 1.0f)},
		};
		std::vector<uint16_t> indices = {
			0,1,2,
			2,1,3
		};
		auto drawComp = AddComponent<PCTSpriteDraw>(vertices, indices);
		drawComp->SetTextureResource(L"numbers");

		SetAlphaActive(true);

	}

	void HideRank::OnCreate()
	{
		float fNumber = static_cast<float>(m_rankNumber);
		Col4 color(1.0f, 1.0f, 1.0f, 1.0f);
		vertices = {
			{Vec3(0.0f,    0.0f,0.0f),color,Vec2((fNumber + 0) * 100.0f / 512.0f, 0.0f)},
			{Vec3(100.0f,   0.0f,0.0f),color,Vec2((fNumber + 1) * 100.0f / 512.0f, 0.0f)},
			{Vec3(0.0f, -100.0f,0.0f),color,Vec2((fNumber + 0) * 100.0f / 512.0f, 1.0f)},
			{Vec3(100.0f,-100.0f,0.0f),color,Vec2((fNumber + 1) * 100.0f / 512.0f, 1.0f)},
		};
		std::vector<uint16_t> indices = {
			0,1,2,
			2,1,3
		};
		auto drawComp = AddComponent<PCTSpriteDraw>(vertices, indices);
		drawComp->SetTextureResource(L"HideRank");

		SetAlphaActive(true);
	}
}