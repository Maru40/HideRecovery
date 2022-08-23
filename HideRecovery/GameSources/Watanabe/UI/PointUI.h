#pragma once
#include "stdafx.h"
#include "Numbers.h"

namespace basecross {
	class PointUI :public GameObject {
		shared_ptr<Numbers> m_numbers;
	public:
		PointUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
