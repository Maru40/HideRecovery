#pragma once
#include "stdafx.h"

namespace basecross {
	class WatanabeStage :public Stage {
		//ビューの作成
		void CreateViewLight();
		shared_ptr<GameObject> m_obj;
		float m_delta = 0;
	public:
		WatanabeStage() :Stage() {}

		void OnCreate() override;
		void OnUpdate() override;
	};
}
