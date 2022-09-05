#include "stdafx.h"
#include "RelocationCountBP.h"
#include "Maruyama/Utility/Component/BillBoard.h"
#include "../Utility/Utility.h"

namespace basecross {
	RelocationCountBP::RelocationCountBP(const shared_ptr<Stage>& stage)
		:StageObjectBase(stage, L"RelocationCountBP")
	{}

	void RelocationCountBP::OnCreate() {
		Vec2 origin(0.0f);
		auto size = Vec2(1);
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

		// 何故か画面に映らない
		m_drawComp = AddComponent<PCTStaticDraw>();
		auto drawComp = m_drawComp.lock();
		drawComp->SetMeshResource(meshRes);
		drawComp->SetTextureResource(L"HPGauge_TX");

		//auto billBoard = AddComponent<BillBoard>();
		//billBoard->SetRotationOffset(
			//Utility::ConvertDegVecToRadVec(Vec3(0, 0, 180)));

		//SetAlphaActive(true);
	}
	void RelocationCountBP::OnUpdate() {
	}
}