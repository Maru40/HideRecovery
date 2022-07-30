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

		float x = 5.0f; // �Q�[�W�̕�
		float y = 32.5f; // �Q�[�W�̍���

		if (m_DamageGauge_x <= 0)
		{
			y = 0.0f;
		}

		// �Q�[�W�̒��g
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