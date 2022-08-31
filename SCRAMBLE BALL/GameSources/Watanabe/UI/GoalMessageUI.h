#pragma once
#include "stdafx.h"
#include "MessageUI.h"
#include "SimpleSprite.h"

namespace basecross {
	class GoalMessageUI :public MessageUI {
		shared_ptr<SimpleSprite> m_sprite;
	public:
		GoalMessageUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void Start()override;
		void Reset()override;
	};
}
