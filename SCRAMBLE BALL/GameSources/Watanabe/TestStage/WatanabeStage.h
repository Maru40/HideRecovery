#pragma once
#include "stdafx.h"

namespace basecross {
	class WatanabeStage :public Stage {
		//ビューの作成
		void CreateViewLight();
	public:
		WatanabeStage() :Stage() {}

		void OnCreate() override;
		void OnUpdate() override;
	};
}
