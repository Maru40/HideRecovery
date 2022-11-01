#pragma once
#include "stdafx.h"
#include "../Utility/GameController.h"
#include "../Effekseer/EfkEffect.h"

namespace basecross {
	class TestEffectStage :public Stage {
		GameController m_controller;
		shared_ptr<EfkComponent> m_efkComp;
		void CreateViewLight();
	public:
		TestEffectStage() :Stage(), m_controller(0) {}

		void OnCreate()override;
		void OnUpdate()override;
	};
}