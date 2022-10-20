#pragma once
#include "stdafx.h"

namespace basecross {
	class TitleCamera :public Camera {
		float m_camY;
		float m_speed;
		float m_range;
		float m_totalDelta;
	public:
		TitleCamera();
		TitleCamera(const Vec3& eye, const Vec3& at);

		void OnCreate()override;
		void OnUpdate()override;
		void SetSpeed(float speed) { m_speed = speed; }
	};
}
