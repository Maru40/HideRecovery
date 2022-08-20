#pragma once
#include "stdafx.h"
#include "../Interface/I_Performable.h"

namespace basecross {
	class WatanabeStage :public Stage {
		//ビューの作成
		void CreateViewLight();
		shared_ptr<GameObject> m_obj;
		shared_ptr<I_Performable> m_per;
		float m_delta = 0;
	public:
		WatanabeStage() :Stage() {}

		void OnCreate() override;
		void OnUpdate() override;
	};
}
