/*!
@file   PlayerSpawnPoint.h
@brief  プレイヤーのスポーン場所クラス
*/

#pragma once
#include "stdafx.h"
#include "StageObjectBase.h"

namespace basecross {
	enum class Team {
		East,
		West
	};

	class PlayerSpawnPointObject :public StageObjectBase {
		// チーム
		Team m_team;
		// 固有ID
		int m_id;
	public:
		PlayerSpawnPointObject(const shared_ptr<Stage>& stage);
		PlayerSpawnPointObject(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
