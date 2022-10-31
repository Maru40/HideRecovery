#pragma once
#include "stdafx.h"

namespace basecross {
	class TestEffectStage :public Stage {
		void CreateViewLight();
	public:
		TestEffectStage() :Stage() {}

		void OnCreate()override;
		void OnUpdate()override;
	};
}