#pragma once
#include "stdafx.h"
#include "StageObjectBase.h"

namespace basecross {
	enum class Team {
		East,
		West
	};

	class PlayerSpawnPoint :public StageObjectBase {
		Team m_team;
		int m_id;
	public:
		PlayerSpawnPoint(const shared_ptr<Stage>& stage);
		PlayerSpawnPoint(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
