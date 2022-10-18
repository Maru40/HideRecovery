
/*!
@file ArrowObject.cpp
@brief ArrowObject�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "ArrowObject.h"
#include "Maruyama/UI/BuilderVertexPCT.h"

namespace basecross {

	void ArrowObject::OnCreate() {
		auto texture = L"Arrow_TX";
		auto build = Builder::BuilderVertexPCT(Builder::VertexPCTParametor(Vec3(0.5f, 0.5f, 0.0f), Vec2(512.0f, 256.0f), texture));

		auto draw = AddComponent<PCTStaticDraw>();
		draw->CreateOriginalMesh(build.m_vertices, build.m_indices);
		draw->SetTextureResource(texture);
		draw->SetOriginalMeshUse(true);
		draw->SetDepthStencilState(DepthStencilState::Read);
		SetAlphaActive(true);
	}

}

//endbasecross