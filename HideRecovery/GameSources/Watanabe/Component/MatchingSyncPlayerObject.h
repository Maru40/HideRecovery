#pragma once
#include "stdafx.h"
#include "Itabashi/OnlineManager.h"
#include "Itabashi/OnlineMatching.h"
#include "../StageObject/PlayerSpawnPointObject.h"
#include "PlayerObject.h"

namespace basecross {
	class MatchingSyncPlayerObject :public Online::OnlineComponent {
		vector<shared_ptr<PlayerSpawnPointObject>> m_spawnPoints;
		shared_ptr<Online::OnlineMatching> m_onlineMatching;
		shared_ptr<GameObject> CreatePlayerModel();
	public:
		MatchingSyncPlayerObject(const shared_ptr<GameObject>& owner,
			const vector<shared_ptr<PlayerSpawnPointObject>>& pointObjects,
			shared_ptr<Online::OnlineMatching> onlineMatching);

		void OnCreate()override;

		// 部屋を作成
		void OnCreateRoom()override;
		// 部屋に参加
		void OnJoinRoom()override;
	};
}
