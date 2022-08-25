﻿#include "stdafx.h"
#include "HPGaugeBoardPoly.h"
#include "BillBoard.h"
#include "../Utility/Utility.h"

namespace basecross {
	HPGaugeBoardPoly::HPGaugeBoardPoly(const shared_ptr<Stage>& stage)
		:StageObjectBase(stage, L"HPGaugeBoardPoly")
	{}

	void HPGaugeBoardPoly::OnCreate() {
		Vec2 origin(0.0f);
		auto size = Vec2(1, 0.2f);
		// テクスチャ座標でUVを定義
		vector<Vec2> uvs, uv = {
			origin,
			origin + Vec2(size.x,0.0f),
			origin + Vec2(0.0f, size.y),
			origin + size
		};
		Col4 color(1.0f);
		Vec2 halfSize = size / 2.0f;
		// 頂点のデータ (番号は左上から右下まで)
		vector<VertexPositionColorTexture> vertices = {
			{Vec3(-halfSize.x,+halfSize.y,0.0f),color,Vec2(0,0)}, //0
			{Vec3(+halfSize.x,+halfSize.y,0.0f),color,Vec2(1,0)}, //1

			{Vec3(-halfSize.x,-halfSize.y,0.0f),color,Vec2(0,1)}, //2
			{Vec3(+halfSize.x,-halfSize.y,0.0f),color,Vec2(1,1)},  //3
		};
		// 頂点インデックス
		vector<uint16_t> indices = {
			0, 1, 2, // 上の三角形
			2, 1, 3  // 下の三角形
		};
		auto meshRes = MeshResource::CreateMeshResource(vertices, indices, true);

		m_drawComp = AddComponent<PCTGaugeDraw>();
		auto drawComp = m_drawComp.lock();
		drawComp->SetMeshResouce(meshRes);
		drawComp->SetTextureResouce(L"HPGauge_TX");
		drawComp->IsBackground(true);
		drawComp->SetGaugeColor(Col4(0, 1, 0, 1));

		auto billBoard = AddComponent<BillBoard>();
		billBoard->SetRotationOffset(
			Utility::ConvertDegVecToRadVec(Vec3(0, 0, 180)));

		SetAlphaActive(true);
	}
	void HPGaugeBoardPoly::OnUpdate() {
		m_drawComp.lock()->SetRate(0.5);
	}
}