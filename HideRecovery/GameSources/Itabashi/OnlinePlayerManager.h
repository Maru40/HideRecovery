#pragma once
#include "stdafx.h"
#include "OnlineManager.h"
#include <unordered_map>

namespace basecross
{
	class PlayerObject;

namespace Online
{
	class OnlinePlayerManager : public OnlineComponent
	{
	public:
		static constexpr int MAX_PLAYER_NUM = 6;

	private:
		bool m_numberUses[MAX_PLAYER_NUM];

		std::vector<std::weak_ptr<PlayerObject>> m_players;

		std::vector<std::shared_ptr<PlayerObject>> GetPlayers();

		std::shared_ptr<PlayerObject> CreatePlayerObject(int playerManager, const Vec3& position);
	public:

		OnlinePlayerManager(const std::shared_ptr<GameObject>& owner);
		
		void OnUpdate() override {}

		void OnDraw() override{}

		void OnJoinRoomEventAction(int playerNumber, const std::vector<int>& playerNumbers, const ExitGames::LoadBalancing::Player& player) override;
		void OnLeaveRoomEventAction(int playerNumber, bool isInactive) override;
	};
}
}