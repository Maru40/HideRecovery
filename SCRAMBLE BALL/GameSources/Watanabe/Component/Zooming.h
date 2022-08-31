#pragma once
#include "stdafx.h"

namespace basecross {
	class Zooming :public Component {
		weak_ptr<RectTransform> m_rectTransform;
		Vec2 m_originalScale;
		float m_totalDelta;
	public:
		Zooming(const shared_ptr<GameObject>& owner);

		void OnCreate()override;
		void OnUpdate()override;
		void OnDraw()override {}
	};
}
