#pragma once
#include "stdafx.h"

namespace basecross {
	class TimeLine :public Component {
	public:
		TimeLine(const shared_ptr<GameObject>& owner);

		void OnCreate()override;
		void OnUpdate()override;
		void OnDraw()override {}
	};
}
