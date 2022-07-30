#include "stdafx.h"
#include "DamageGauge.h"
#include "Scene.h"

namespace basecross
{
	void DamageGauge::OnCreate()
	{

	}

	void DamageGauge::OnUpdate()
	{

		auto scene = App::GetApp()->GetScene<Scene>();
		m_DamageGauge_x = scene->GetPlayerHP() * 2.15f;

		float x = 5.0f; // ゲージの幅
		float y = 32.5f; // ゲージの高さ

		if (m_DamageGauge_x <= 0)
		{
			y = 0.0f;
		}

		// ゲージの中身
		auto Comp = AddComponent<StringSprite>();
		Comp->SetTextRect(Rect2D<float>(10, +5.0f, +m_DamageGauge_x + 10, +5.0f + y));
		// Comp->SetTextRect(Rect2D<float>(x + 4.5f, +200.25f, +m_PHpgauge_x + 4.5f, +200.25f + y));
		Comp->SetBackColor(Col4(0.9f, 0.0f, 0.0f, 0.9f));

	}

	void DamageGauge::OnDraw()
	{
		GameObject::OnDraw();
	}

}