﻿/*!
@file   PlayerSpawnPoint.cpp
@brief  プレイヤーのスポーン場所クラス実体
*/

#include "stdafx.h"
#include "PlayerSpawnPointObject.h"
#include "../Utility/DataExtracter.h"
#include "../DebugClass/Debug.h"

#include "Maruyama/Player/Component/PlayerSpawnPoint.h"

namespace basecross {
	PlayerSpawnPointObject::PlayerSpawnPointObject(const shared_ptr<Stage>& stage)
		:StageObjectBase(stage, L"PlayerSpawnPoint"), m_id(0), m_team(team::TeamType::Blue)
	{}

	PlayerSpawnPointObject::PlayerSpawnPointObject(const shared_ptr<Stage>& stage, const wstring& line)
		: StageObjectBase(stage, L"PlayerSpawnPoint")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		size_t nextIndex = DataExtracter::TransformDataExtraction(tokens, m_transformData);

		wstring teamType = tokens[nextIndex];
		if (teamType == L"Blue") {
			m_team = team::TeamType::Blue;
		}
		else if (teamType == L"Red") {
			m_team = team::TeamType::Red;
		}
		else {
			throw BaseException(
				L"Teamが不正な値です。",
				L"team::TeamType : " + teamType,
				L"PlayerSpawnPoint::PlayerSpawnPoint()"
			);
		}

		++nextIndex;

		m_id = std::stoi(tokens[nextIndex]);
	}

	void PlayerSpawnPointObject::OnCreate() {
		AddComponent<PlayerSpawnPoint>(PlayerSpawnPoint::Parametor(m_team, m_id));
	}
	void PlayerSpawnPointObject::OnUpdate() {
	}
}