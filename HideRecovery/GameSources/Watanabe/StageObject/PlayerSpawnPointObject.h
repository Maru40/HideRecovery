/*!
@file   PlayerSpawnPoint.h
@brief  プレイヤーのスポーン場所クラス
*/

#pragma once
#include "stdafx.h"
#include "StageObjectBase.h"
#include "Maruyama/Interface/I_TeamMember.h"

namespace basecross {
	class PlayerSpawnPointObject :public StageObjectBase {
		// チーム
		team::TeamType m_team;
		// 固有ID
		int m_id;
	public:
		PlayerSpawnPointObject(const shared_ptr<Stage>& stage);
		PlayerSpawnPointObject(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
