#pragma once
#include "stdafx.h"
#include "MessageUI.h"

namespace basecross {
	class GoalMessageUI :public MessageUI {
	public:
		GoalMessageUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void Start()override;
	};
}
