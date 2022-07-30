#include "stdafx.h"
#include "Reticle.h"
#include "Utility.h"

namespace basecross {
	void Reticle::OnCreate()
	{
		Col4 color(1.0f, 1.0f, 0.0f, 1.0f); // 照準の色
		float size = 50.0f; // 照準の大きさ（本当はメンバ変数にしてコンストラクタで設定するべき）
		std::vector<VertexPositionColorTexture> vertices = {
			{Vec3(-0.5f, +0.5f, 0.0f) * size, color, Vec2(0.0f, 0.0f)},
			{Vec3(+0.5f, +0.5f, 0.0f) * size, color, Vec2(1.0f, 0.0f)},
			{Vec3(-0.5f, -0.5f, 0.0f) * size, color, Vec2(0.0f, 1.0f)},
			{Vec3(+0.5f, -0.5f, 0.0f) * size, color, Vec2(1.0f, 1.0f)},
		};
		std::vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3
		};
		auto drawComp = AddComponent<PCTSpriteDraw>(vertices, indices);
		drawComp->SetTextureResource(L"reticle");

		SetAlphaActive(true); // 透明色に対応させる
	}

	void Reticle::OnUpdate()
	{
		const auto& stage = GetStage(); // ステージを取得する

		auto targetPos = target->GetComponent<Transform>()->GetPosition(); // ターゲットのワールド座標を取得する
		auto target2DPos = Utility::ConvertWorldToScreen(stage->GetView(), targetPos); // ターゲットの座標をスクリーン座標に変換する

		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(target2DPos); // 照準をターゲットの二次元座標に移動させる
	}
}