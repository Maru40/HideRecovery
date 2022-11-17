#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"

namespace basecross {
	class InProcessUI;
	class SplashMessageUI;

	class ReconnectingInGameUI :public UIObjectBase {
		weak_ptr<InProcessUI> m_inProcessUI;
		vector<weak_ptr<UIObjectBase>> m_toTitleUIs;
		weak_ptr<SplashMessageUI> m_splashMessageUI;
	public:
		ReconnectingInGameUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override {}

		void SetSplashMessageUI(const shared_ptr<SplashMessageUI>& ui) { m_splashMessageUI = ui; }
	};
}
