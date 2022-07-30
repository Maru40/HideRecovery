#include "stdafx.h"
#include "EnergeyGauge.h"
#include "Scene.h"

namespace basecross
{
	void EnergeyGauge::OnCreate()
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
		drawComp->SetTextureResource(L"energybarback");

		SetAlphaActive(true);

		auto transComp = GetComponent<Transform>();
		Vec3 pos(-600.0f, -340.0f, 0.0f);
		transComp->SetScale(0.3f, 0.3f, 0.3f);
		transComp->SetPosition(pos);

	}

	void EnergeyGauge::OnUpdate()
	{
		auto scene = App::GetApp()->GetScene<Scene>();
		float deltatime = App::GetApp()->GetElapsedTime();

		if (m_Egauge_x <= 0)
		{
			m_counttime += deltatime;
		}

		if (m_counttime < 0.5f || m_Egauge_x > 0)
		{
			SetDrawActive(true);
		}
		else if(m_counttime >= 0.5f && m_counttime < 1.0f)
		{
			SetDrawActive(false);
		}
		else
		{
			m_counttime = 0;
		}

		m_Egauge_x = scene->GetPlayerEnergy() * 3.45f;

		float x = 20.0f; // ゲージの幅
		float y = 25.5f; // ゲージの高さ

		if (m_Egauge_x <= 0)
		{
			y = 0.0f;
		}


		// ゲージの中身
		auto Comp = AddComponent<StringSprite>();
		Comp->SetTextRect(Rect2D<float>(85, +747.0f, +m_Egauge_x + 85, +747.0f + y));
 		// Comp->SetTextRect(Rect2D<float>(x + 4.5f, +260.25f, +m_Egauge_x + 4.5f, +260.25f + y));
		Comp->SetBackColor(Col4(0.0f, 1.0f, 1.0f, 1.0f));

	}

	//void EnergeyGauge::OnDraw()
	//{
	//	if (GetDrawActive())
	//	{
	//		GameObject::OnDraw();
	//	}
	//}
}