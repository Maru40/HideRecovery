#pragma once
#include "stdafx.h"
#include "StageObjectBase.h"
#include "PlayerSpawnPoint.h"

namespace basecross {
	class OwnArea :public StageObjectBase {
		float m_areaRadius;
		Team m_team;
	public:
		OwnArea(const shared_ptr<Stage>& stage);
		OwnArea(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
