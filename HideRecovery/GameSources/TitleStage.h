#pragma once
#include "stdafx.h"

namespace basecross {
	class TitleStage :public Stage {
		void CreateViewLight();
	public:
		TitleStage() :Stage() {}

		void OnCreate()override;
		void OnUpdate()override;
		void OnDestroy()override;
	};
}
