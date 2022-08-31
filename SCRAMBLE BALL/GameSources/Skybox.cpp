/*!
@file Skybox.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "SkyBox.h"

namespace basecross{
	void Skybox::OnCreate()
	{
		std::vector<VertexPositionTexture> vertices = {
			{Vec3(-0.5f, +0.5f, +0.5f), Vec2(0.0f, 0.25f)},
			{Vec3(+0.5f, +0.5f, +0.5f), Vec2(0.25f, 0.25f)},
			{Vec3(-0.5f, -0.5f, +0.5f), Vec2(0.0f, 0.5f)},
			{Vec3(+0.5f, -0.5f, +0.5f), Vec2(0.25f, 0.5f)},

			{Vec3(+0.5f, +0.5f, +0.5f), Vec2(0.25f, 0.25f)},
			{Vec3(+0.5f, +0.5f, -0.5f), Vec2(0.5f, 0.25f)},
			{Vec3(+0.5f, -0.5f, +0.5f), Vec2(0.25f, 0.5f)},
			{Vec3(+0.5f, -0.5f, -0.5f), Vec2(0.5f, 0.5f)},

			{Vec3(+0.5f, +0.5f, -0.5f), Vec2(0.5f, 0.25f)},
			{Vec3(-0.5f, +0.5f, -0.5f), Vec2(0.75f, 0.25f)},
			{Vec3(+0.5f, -0.5f, -0.5f), Vec2(0.5f, 0.5f)},
			{Vec3(-0.5f, -0.5f, -0.5f), Vec2(0.75f, 0.5f)},

			{Vec3(-0.5f, +0.5f, -0.5f), Vec2(0.75f, 0.25f)},
			{Vec3(-0.5f, +0.5f, +0.5f), Vec2(1.0f, 0.25f)},
			{Vec3(-0.5f, -0.5f, -0.5f), Vec2(0.75f, 0.5f)},
			{Vec3(-0.5f, -0.5f, +0.5f), Vec2(1.0f, 0.5f)},

			{Vec3(-0.5f, +0.5f, -0.5f), Vec2(0.0f, 0.0f)},
			{Vec3(+0.5f, +0.5f, -0.5f), Vec2(0.25f, 0.0f)},
			{Vec3(-0.5f, +0.5f, +0.5f), Vec2(0.0f, 0.25f)},
			{Vec3(+0.5f, +0.5f, +0.5f), Vec2(0.25f, 0.25f)},

			{Vec3(-0.5f, -0.5f, +0.5f), Vec2(0.0f, 0.5f)},
			{Vec3(+0.5f, -0.5f, +0.5f), Vec2(0.25f, 0.5f)},
			{Vec3(-0.5f, -0.5f, -0.5f), Vec2(0.0f, 0.75f)},
			{Vec3(+0.5f, -0.5f, -0.5f), Vec2(0.25f, 0.75f)},
		};

		const std::vector<uint16_t> baseIndices = {
			0, 1, 2, 2, 1, 3
		};

		std::vector<uint16_t> indices;
		for (int i = 0; i < 6; i++) {
			for (auto baseIndex : baseIndices) {
				indices.push_back(baseIndex + i * 4);
			}
		}

		auto drawComp = AddComponent<PTStaticDraw>();
		drawComp->SetOriginalMeshUse(true);
		drawComp->CreateOriginalMesh(vertices, indices);
		drawComp->SetTextureResource(L"skybox");
		//drawComp->SetSamplerState(SamplerState::LinearWrap);

		SetAlphaActive(true);

		auto transComp = GetComponent<Transform>();
		transComp->SetScale(Vec3(707.0f));
	}

	void Skybox::OnUpdate()
	{
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(GetStage()->GetView()->GetTargetCamera()->GetEye());
	}
}
//end basecross
