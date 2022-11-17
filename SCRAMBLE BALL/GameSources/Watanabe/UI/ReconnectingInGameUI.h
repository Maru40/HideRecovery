#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"

namespace basecross {
	class InProcessUI;
	class SimpleSprite;

	class ReconnectingInGameUI :public UIObjectBase {
		weak_ptr<InProcessUI> m_inProcessUI;
		vector<weak_ptr<SimpleSprite>> m_toTitleUIs;
		weak_ptr<SimpleSprite> m_splashMessageUI;
	public:
		ReconnectingInGameUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override {}
	};
}
