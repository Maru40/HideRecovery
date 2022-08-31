#include "stdafx.h"
#include "Zooming.h"

namespace basecross {
	Zooming::Zooming(const shared_ptr<GameObject>& owner)
		:Component(owner), m_totalDelta(0)
	{}

	void Zooming::OnCreate() {
		m_rectTransform = GetGameObject()->GetComponent<RectTransform>();
		m_originalScale = m_rectTransform.lock()->GetScale();
	}

	void Zooming::OnUpdate() {
		if (m_totalDelta > XM_2PI)
			m_totalDelta = 0.0f;

		float scale = sinf(m_totalDelta) * 0.1f;
		auto rectTrans = m_rectTransform.lock();
		rectTrans->SetScale(m_originalScale + Vec2(scale));

		m_totalDelta += App::GetApp()->GetElapsedTime() * 2;
	}
}