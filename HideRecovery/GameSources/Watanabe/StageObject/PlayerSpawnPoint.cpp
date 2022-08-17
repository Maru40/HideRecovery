﻿#include "stdafx.h"
#include "PlayerSpawnPoint.h"
#include "../Utility/DataExtracter.h"

namespace basecross {
	PlayerSpawnPoint::PlayerSpawnPoint(const shared_ptr<Stage>& stage)
		:StageObjectBase(stage, L"PlayerSpawnPoint")
	{}

	PlayerSpawnPoint::PlayerSpawnPoint(const shared_ptr<Stage>& stage, const wstring& line)
		: StageObjectBase(stage, L"PlayerSpawnPoint")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		size_t nextIndex = DataExtracter::TransformDataExtraction(tokens, m_transformData);

		wstring teamType = tokens[nextIndex];
		if (teamType == L"East") {
			m_team = Team::East;
		}
		else if (teamType == L"West") {
			m_team = Team::West;
		}
		else {
			throw BaseException(
				L"Teamが不正な値です。",
				L"Team : " + teamType,
				L"PlayerSpawnPoint::PlayerSpawnPoint()"
			);
		}
	}

	void PlayerSpawnPoint::OnCreate() {
	}
	void PlayerSpawnPoint::OnUpdate() {
	}
}