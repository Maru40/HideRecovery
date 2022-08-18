#pragma once
#include "stdafx.h"

namespace basecross {
	class GameStartUI :public GameObject {
	public:
		GameStartUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
