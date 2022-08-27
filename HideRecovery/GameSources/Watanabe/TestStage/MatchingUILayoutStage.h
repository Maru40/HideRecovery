#pragma once
#include "stdafx.h"
#include "../UI/SplashMessageUI.h"
#include "GameStageBase.h"

namespace basecross {
	class MatchingUILayoutStage :public GameStageBase {
		void CreateViewLight();
		shared_ptr<SplashMessageUI> m_sprite;
	public:
		MatchingUILayoutStage() :GameStageBase() {}

		void OnCreate()override;
		void OnUpdate()override;
	};
}
