#pragma once
#include "stdafx.h"

namespace basecross {
	class MatchingUILayoutStage :public Stage {
		void CreateViewLight();
	public:
		MatchingUILayoutStage() :Stage() {}

		void OnCreate()override;
		void OnUpdate()override;
	};
}
