#include "stdafx.h"
#include "Plane.h"

namespace basecross {
	void Plane::OnCreate()
	{
		// �|�����`�����邽�߂̒��_�f�[�^
		std::vector<VertexPositionTexture> vertices = {
			{Vec3(-0.5f, +0.5f, 0.0f), Vec2(0.0f, 0.0f)},
			{Vec3(+0.5f, +0.5f, 0.0f), Vec2(1.0f, 0.0f)},
			{Vec3(-0.5f, -0.5f, 0.0f), Vec2(0.0f, 1.0f)},
			{Vec3(+0.5f, -0.5f, 0.0f), Vec2(1.0f, 1.0f)}
		};

		// ���_�C���f�b�N�X
		std::vector<uint16_t> indices = {
			0, 1, 2, 2, 1, 3
		};

		// ��L�f�[�^��K�p
		auto drawComp = AddComponent<PTStaticDraw>();
		drawComp->SetOriginalMeshUse(true);
		drawComp->CreateOriginalMesh(vertices, indices);
	}
}
