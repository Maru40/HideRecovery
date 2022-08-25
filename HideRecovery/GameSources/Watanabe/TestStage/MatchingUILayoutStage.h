#pragma once
#include "stdafx.h"
#include "../UI/SplashMessageUI.h"

namespace basecross {
	class MatchingUILayoutStage :public Stage {
		void CreateViewLight();
		shared_ptr<SplashMessageUI> m_sprite;
	public:
		MatchingUILayoutStage() :Stage() {}

		void OnCreate()override;
		void OnUpdate()override;
	};
}
