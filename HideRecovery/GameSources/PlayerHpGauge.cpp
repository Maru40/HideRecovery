#include "stdafx.h"
#include "PlayerHpGauge.h"
#include "Scene.h"

#include "GameStageBase.h"
#include "Player.h"
#include "I_Damaged.h"
#include "I_Pusher.h"
#include "PlayerStatusManager.h"

namespace basecross
{
	void PlayerHpgauge::OnCreate()
	{
		Col4 color(1.0f, 1.0f, 1.0f, 1.0f);
		std::vector<VertexPositionColorTexture> vertices = {
			{Vec3(0.0f,    0.0f, 0.0f), color, Vec2(0.0f, 0.0f)},
			{Vec3(1356.0f,  0.0f, 0.0f), color, Vec2(1.0f, 0.0f)},
			{Vec3(0.0f, -130.0f, 0.0f), color, Vec2(0.0f, 1.0f)},
			{Vec3(1356.0f, -130.0f, 0.0f), color, Vec2(1.0f, 1.0f)}
		};
		std::vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3
		};

		auto drawComp = AddComponent<PCTSpriteDraw>(vertices, indices);
		drawComp->SetTextureResource(L"hpbarback");

		SetAlphaActive(true);

		auto transComp = GetComponent<Transform>();
		Vec3 pos(-600.0f, -300.0f, 0.0f);
		transComp->SetScale(0.3f, 0.3f, 0.3f);
		transComp->SetPosition(pos);

	}

	void PlayerHpgauge::OnUpdate()
	{
		auto stage = GetGameStage();
		if (!stage) {
			return;
		}
		auto player = stage->GetPlayer();
		auto statusManager = player->GetComponent<PlayerStatusManager>(false);
		if (statusManager) {
			auto scene = App::GetApp()->GetScene<Scene>();
			//m_PHpgauge_x = scene->GetPlayerHP() * 3.8f;
			m_PHpgauge_x = statusManager->GetStatus().hp * 3.45f;
		}

		float x = 20.0f; // ゲージの幅
		float y = 25.5f; // ゲージの高さ

		if (m_PHpgauge_x <= 0)
		{
			y = 0.0f;
		}


		// ゲージの中身
		auto Comp = AddComponent<StringSprite>();
		Comp->SetTextRect(Rect2D<float>(85, +707.0f, +m_PHpgauge_x + 85, +707.0f + y));
		// Comp->SetTextRect(Rect2D<float>(x + 4.5f, +200.25f, +m_PHpgauge_x + 4.5f, +200.25f + y));
		Comp->SetBackColor(Col4(0.0f, 1.0f, 0.0f, 1.0f));

	}

	void PlayerHpgauge::OnDraw()
	{
		GameObject::OnDraw();
	}
}