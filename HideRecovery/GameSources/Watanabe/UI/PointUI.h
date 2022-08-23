#pragma once
#include "stdafx.h"
#include "Numbers.h"
#include "../StageObject/PlayerSpawnPointObject.h"

namespace basecross {
	class PointUI :public GameObject {
		shared_ptr<Numbers> m_numbers;
		Team m_team;
	public:
		PointUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
