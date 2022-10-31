#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"

namespace basecross {
	class DirectionWithHasBallUI :public UIObjectBase {
	public:
		DirectionWithHasBallUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
