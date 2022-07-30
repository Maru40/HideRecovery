#include "stdafx.h"
#include "SinScaling.h"

namespace basecross {
	void SinScaling::OnCreate()
	{
		m_firstScale = GetGameObject()->GetComponent<Transform>()->GetScale();
	}

	void SinScaling::OnUpdate()
	{
		Scaling();
	}
	 
	void SinScaling::Scaling()
	{
		auto& app = App::GetApp();
		auto delta = app->GetElapsedTime();
		m_delta += delta * m_deltaSpeed;
		if (m_delta >= XM_PI)
		{
			m_delta = 0.0f;
		}
		m_scale = Vec3(m_minScale) + Vec3(sinf(m_delta)) * 0.25f * m_scaleAdjust;

		auto obj = GetGameObject();
		auto transComp = obj->GetComponent<Transform>();
		transComp->SetScale(m_scale);
	}

	void SinScaling::Reset() {
		m_delta = 0.0f;
		GetGameObject()->GetComponent<Transform>()->SetScale(m_firstScale);
	}
}