#include "stdafx.h"
#include "HPGaugeBP.h"
#include "BillBoard.h"
#include "../Component/SyncObject.h"
#include "../Utility/Utility.h"

namespace basecross {
	HPGaugeBP::HPGaugeBP(const shared_ptr<Stage>& stage, const shared_ptr<PlayerStatus>& status)
		: StageObjectBase(stage, L"HPGaugeBP"), m_status(status)
	{}

	void HPGaugeBP::OnCreate() {
		auto size = Vec2(1, 0.1f);
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
		m_meshRes = MeshResource::CreateMeshResource(vertices, indices, true);

		m_drawComp = AddComponent<PCTGaugeDraw>();
		auto drawComp = m_drawComp.lock();
		drawComp->SetMeshResource(m_meshRes);
		drawComp->SetTextureResource(L"HPGauge_TX");
		drawComp->IsBackground(true);
		drawComp->SetGaugeGradientTexture(L"HPColorGauge_TX");

		auto billBoard = AddComponent<BillBoard>();
		billBoard->SetRotationOffset(
			Utility::ConvertDegVecToRadVec(Vec3(0, 0, 180)));

		auto targetTrans = m_status.lock()->GetGameObject()->GetComponent<Transform>();
		auto syncComp = AddComponent<SyncObject>();
		syncComp->SetTarget(targetTrans);
		syncComp->SetOffsetPosition(Vec3(0, 1.5f, 0));

		SetAlphaActive(true);
	}
	void HPGaugeBP::OnUpdate() {
		auto status = m_status.lock()->GetStatus();
		m_drawComp.lock()->SetRate(status.hp / (float)status.maxHp);
	}
}