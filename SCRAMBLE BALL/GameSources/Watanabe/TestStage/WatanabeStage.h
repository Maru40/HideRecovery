#pragma once
#include "stdafx.h"

namespace basecross {
	class ReconnectingInGameUI;

	class WatanabeStage :public Stage {
		shared_ptr<ReconnectingInGameUI> m_ui;
		//ビューの作成
		void CreateViewLight();
	public:
		WatanabeStage() :Stage() {}

		void OnCreate() override;
		void OnUpdate() override;
	};
}
