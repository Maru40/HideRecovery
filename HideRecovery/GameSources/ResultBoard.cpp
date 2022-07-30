#include "stdafx.h"
#include "ResultBoard.h"

namespace basecross
{
	void ResultBoard::OnCreate()
	{
		std::vector<VertexPositionTexture> vertices = {
			{Vec3(0.0f,    0.0f, 0.0f), Vec2(0.0f, 0.0f)},
			{Vec3(1280.0f,    0.0f, 0.0f), Vec2(1.0f, 0.0f)},
			{Vec3(0.0f, -800.0f, 0.0f), Vec2(0.0f, 1.0f)},
			{Vec3(1280.0f, -800.0f, 0.0f), Vec2(1.0f, 1.0f)}
		};
		std::vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3
		};

		auto drawComp = AddComponent<PTSpriteDraw>(vertices, indices);
		drawComp->SetTextureResource(L"ResultBackGround");

		SetAlphaActive(true);

		auto transComp = AddComponent<Transform>();
		Vec3 pos(-640.0f, 400.0f, 0.0f);
		transComp->SetScale(1.0f, 1.0f, 1.0f); // ‘å‚«‚³‚ðŽw’è
		transComp->SetPosition(pos); // ˆÊ’u‚ðŽw’è

		SetDrawLayer(-1);
	}

	void ResultBoard::OnDraw()
	{
		GameObject::OnDraw();
	}
}