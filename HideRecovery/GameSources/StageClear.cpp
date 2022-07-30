#include "stdafx.h"
#include "StageClear.h"

namespace basecross {
	void StageClear::OnCreate()
	{
		Col4 color(1.0f, 1.0f, 1.0f, 1.0f);
		std::vector<VertexPositionColorTexture> vertices = {
			{Vec3(0.0f,    0.0f, 0.0f), color, Vec2(0.0f / 870.0f,           0.0f / 106.0f)},
			{Vec3(870.0f,   0.0f, 0.0f), color, Vec2(870.0f / 870.0f,         0.0f / 106.0f)},
			{Vec3(0.0f, -106.0f, 0.0f), color, Vec2(0.0f / 870.0f, 106.0f / 106.0f)},
			{Vec3(870.0f, -106.0f, 0.0f), color, Vec2(870.0f / 870.0f, 106.0f / 106.0f)}
		};
		std::vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3
		};
		m_drawComp = AddComponent<PCTSpriteDraw>(vertices, indices);
		m_drawComp->SetTextureResource(L"clear");

		SetAlphaActive(true);

		auto transComp = AddComponent<Transform>();
		Vec3 pos(-400.0f, 70.0f, 0.0f);
		transComp->SetPosition(pos);
	}


	void StageClear::OnDraw()
	{
		GameObject::OnDraw();
	}
}