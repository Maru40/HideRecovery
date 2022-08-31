#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"

namespace basecross {
	class HoldButtonUI :public UIObjectBase {
	public:
		HoldButtonUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
