#pragma once
#include "stdafx.h"
#include "Itabashi/OnlineManager.h"
#include "Itabashi/OnlineMatching.h"
#include "../StageObject/PlayerSpawnPointObject.h"
#include "StageObjectBase.h"

namespace basecross {
	class MatchingSyncPlayerObject :public Online::OnlineComponent {
		vector<shared_ptr<StageObjectBase>> m_players;
		shared_ptr<Online::OnlineMatching> m_onlineMatching;
		shared_ptr<StageObjectBase> CreatePlayerModel();
	public:
		MatchingSyncPlayerObject(const shared_ptr<GameObject>& owner,
			const vector<shared_ptr<PlayerSpawnPointObject>>& pointObjects,
			shared_ptr<Online::OnlineMatching> onlineMatching);

		void OnCreate()override;

		// 誰かが部屋に参加
		void OnJoinRoomEventAction(int playerNumber, const std::vector<int>& playerNumbers, const ExitGames::LoadBalancing::Player& player)override;
		// 誰かが部屋を抜けた
		void OnLeaveRoomEventAction(int playerNumber, bool isInactive)override;
	};
}
