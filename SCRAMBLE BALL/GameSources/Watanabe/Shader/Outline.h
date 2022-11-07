#pragma once
#include "stdafx.h"

namespace basecross {
	class Outline :public DrawComponent {
	public:
		Outline(const shared_ptr<GameObject>& owner);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
