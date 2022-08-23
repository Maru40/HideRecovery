#pragma once
#include "stdafx.h"

namespace basecross {
	class HPGaugeUI :public GameObject {
	public:
		HPGaugeUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
