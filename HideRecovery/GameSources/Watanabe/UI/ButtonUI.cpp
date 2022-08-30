#include "stdafx.h"
#include "ButtonUI.h"

namespace basecross {
	ButtonUI::ButtonUI(const shared_ptr<Stage>& stage)
		:UIObjectBase(stage, L"ButtonUI")
	{}
	ButtonUI::ButtonUI(const shared_ptr<Stage>& stage, const wstring& line)
		: UIObjectBase(stage, L"ButtonUI")
	{}

	void ButtonUI::OnCreate() {
	}

	void ButtonUI::SetActive(bool flg) {
	}
}