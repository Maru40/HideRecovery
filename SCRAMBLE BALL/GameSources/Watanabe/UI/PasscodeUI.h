#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"
#include "SimpleSprite.h"
#include "Numbers.h"

namespace basecross {
	/// <summary>
	/// パスワード入力用UIオブジェクト
	/// </summary>
	class PasscodeUI :public UIObjectBase
	{
	public:
		PasscodeUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
	};
}
