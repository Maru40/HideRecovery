#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"

namespace basecross {
	class ButtonUI :public UIObjectBase {
	public:
		ButtonUI(const shared_ptr<Stage>& stage);
		ButtonUI(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;

		void SetActive(bool flg);
		void ChangeButton() {}
	};
}
