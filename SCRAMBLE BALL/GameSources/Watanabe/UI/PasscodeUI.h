#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"
#include "SimpleSprite.h"
#include "Numbers.h"

namespace basecross {
	class PasscodeUI :public UIObjectBase {
		shared_ptr<SimpleSprite> m_label;
		shared_ptr<Numbers> m_passCodeNumbers;
	public:
		PasscodeUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void UpdateCode(int code);
	};
}
