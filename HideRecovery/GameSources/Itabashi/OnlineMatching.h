#pragma once
#include "stdafx.h"
#include "OnlineManager.h"
#include <unordered_map>

namespace basecross
{
namespace Online
{
	class OnlineMatching : public OnlineComponent
	{
	public:
		static constexpr int MAX_PLAYER_NUM = 6;

		static constexpr std::uint8_t EXECUTE_UPDATE_PLAYER_NUMBERS = 101;
		
	private:

		static int m_playerNumbers[MAX_PLAYER_NUM];
		//static std::unordered_map<int,int> m_player

		void PlayerStateUpdateEvent(int playerNumber, bool isAdd);

		void ExecuteGetPlayerStateEvent(int playerNumbers[]);

	public:
		OnlineMatching(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;

		void OnCreateRoom() override;

		void OnJoinRoom() override;

		void OnJoinRoomEventAction(int playerNumber, const std::vector<int>& playerNumbers, const ExitGames::LoadBalancing::Player& player) override;

		void OnLeaveRoomEventAction(int playerNumber, bool isInactive) override;

		void OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes) override;

		bool IsFullyOccupied() const;

		int GetPlayerCount() const;

		void Reset();
	};
}
}