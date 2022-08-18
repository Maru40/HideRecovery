/*!
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
		:StageObjectBase(stage, L"PlayerSpawnPoint"), m_id(0), m_team(Team::East)
	{}

	PlayerSpawnPointObject::PlayerSpawnPointObject(const shared_ptr<Stage>& stage, const wstring& line)
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

		++nextIndex;

		m_id = std::stoi(tokens[nextIndex]);
	}

	void PlayerSpawnPointObject::OnCreate() {
		Debug::GetInstance()->Log(L"Create PlayerSpawnPoint");

		AddComponent<PlayerSpawnPoint>(PlayerSpawnPoint::Parametor(m_team, m_id));
	}
	void PlayerSpawnPointObject::OnUpdate() {
	}
}