#include "stdafx.h"
#include "TitleCamera.h"
#include "../DebugClass/Debug.h"

namespace basecross {
	TitleCamera::TitleCamera()
		:Camera(), m_totalDelta(0), m_speed(0.1f), m_camY(0), m_range(0)
	{}
	TitleCamera::TitleCamera(const Vec3& eye, const Vec3& at)
		: Camera(), m_totalDelta(0), m_speed(0.1f), m_camY(0), m_range(0)
	{
		SetEye(eye);
		SetAt(at);
	}

	void TitleCamera::OnCreate() {
		m_camY = GetEye().y;
		m_range = (GetEye() - GetAt()).length();
	}
	void TitleCamera::OnUpdate() {
		if (m_totalDelta > XM_2PI)
			m_totalDelta = 0.0f;

		Vec3 newEye(cosf(m_totalDelta) * m_range, m_camY, sinf(m_totalDelta) * m_range);
		SetEye(newEye);
		m_totalDelta += App::GetApp()->GetElapsedTime() * m_speed;
	}
}