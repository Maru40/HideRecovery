#pragma once
#include "stdafx.h"
#include "../Interface/I_Performable.h"
#include "Watanabe/UI/GameStartUI.h"
#include "Watanabe/UI/GameFinishUI.h"

namespace basecross {
	class WatanabeStage :public Stage {
		//ビューの作成
		void CreateViewLight();
		shared_ptr<GameObject> m_obj;
		shared_ptr<I_Performable> m_per;
		float m_delta = 0;
		std::shared_ptr<GameStartUI> m_gameStartUI;
		std::shared_ptr<GameFinishUI> m_gameFinishUI;

	public:
		WatanabeStage() :Stage() {}

		void OnCreate() override;
		void OnUpdate() override;
	};
}
