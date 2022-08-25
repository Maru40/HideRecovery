#include "stdafx.h"
#include "SplashMessageUI.h"

namespace basecross {
	SplashMessageUI::SplashMessageUI(const shared_ptr<Stage>& stage)
		:UIObjectBase(stage, L"SplashMessageUI")
	{}

	SplashMessageUI::SplashMessageUI(const shared_ptr<Stage>& stage, const wstring& line)
		: UIObjectBase(stage, L"SplashMessageUI")
	{}

	void SplashMessageUI::OnCreate() {
	}

	void SplashMessageUI::SetMessage(const wstring& key) {
	}

	void SplashMessageUI::Start() {
	}
	void SplashMessageUI::Reset() {
	}
}