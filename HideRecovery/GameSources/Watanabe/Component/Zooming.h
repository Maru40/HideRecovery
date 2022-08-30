#pragma once
#include "stdafx.h"

namespace basecross {
	class Zooming :public Component {
	public:
		Zooming(const shared_ptr<GameObject>& owner);

		void OnCreate()override;
		void OnUpdate()override;
		void OnDraw()override {}
	};
}
