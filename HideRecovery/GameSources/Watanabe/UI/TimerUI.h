#pragma once
#include "stdafx.h"
#include "SimpleSprite.h"
#include "Numbers.h"
#include "StageObjectBase.h"

namespace basecross {
	class TimerUI :public StageObjectBase {
		shared_ptr<SimpleSprite> m_sprite;
		shared_ptr<Numbers> m_numbers;
	public:
		TimerUI(const shared_ptr<Stage>& stage);
		TimerUI(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
