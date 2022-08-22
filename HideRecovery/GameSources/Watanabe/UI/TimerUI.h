#pragma once
#include "stdafx.h"
#include "SimpleSprite.h"
#include "Numbers.h"

namespace basecross {
	class TimerUI :public GameObject {
		shared_ptr<SimpleSprite> m_sprite;
		shared_ptr<Numbers> m_numbers;
	public:
		TimerUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
