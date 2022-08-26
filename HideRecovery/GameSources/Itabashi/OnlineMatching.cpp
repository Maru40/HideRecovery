#include "stdafx.h"
#include "OnlineMatching.h"

namespace basecross
{
namespace Online
{
	int OnlineMatching::m_playerNumbers[OnlineMatching::MAX_PLAYER_NUM] = {};

	OnlineMatching::OnlineMatching(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner)
	{

	}

	void OnlineMatching::PlayerStateUpdateEvent(int playerNumber, bool isAdd)
	{
		if (!OnlineManager::GetLocalPlayer().getIsMasterClient())
		{
			return;
		}

		for (auto& number : m_playerNumbers)
		{
			if (isAdd)
			{
				if (number == 0)
				{
					number = playerNumber;
					break;
				}
			}
			else
			{
				if (number == playerNumber)
				{
					number = 0;
					break;
				}
			}
		}

		OnlineManager::RaiseEvent(false, (std::uint8_t*)m_playerNumbers, sizeof(m_playerNumbers), EXECUTE_UPDATE_PLAYER_NUMBERS);
	}

	void OnlineMatching::ExecuteGetPlayerStateEvent(int playerNumbers[])
	{
		std::memcpy(m_playerNumbers, playerNumbers, sizeof(m_playerNumbers));
	}

	void OnlineMatching::OnCreateRoom()
	{
	}

	void OnlineMatching::OnJoinRoom()
	{
	}

	void OnlineMatching::OnJoinRoomEventAction(int playerNumber, const std::vector<int>& playerNumbers, const ExitGames::LoadBalancing::Player& player)
	{
		PlayerStateUpdateEvent(playerNumber, true);
	}

	void OnlineMatching::OnLeaveRoomEventAction(int playerNumber, bool isInactive)
	{
		PlayerStateUpdateEvent(playerNumber, false);
	}

	void OnlineMatching::OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes)
	{
		if (eventCode == EXECUTE_UPDATE_PLAYER_NUMBERS)
		{
			ExecuteGetPlayerStateEvent((int*)bytes);
			return;
		}
	}

	bool OnlineMatching::IsFullyOccupied() const
	{
		auto& room = OnlineManager::GetCurrentlyJoinedRoom();
		return room.getPlayerCount() == room.getMaxPlayers();
	}

	int OnlineMatching::GetPlayerCount() const
	{
		return OnlineManager::GetCurrentlyJoinedRoom().getPlayerCount();
	}

	void OnlineMatching::Reset()
	{
		std::fill_n(m_playerNumbers, MAX_PLAYER_NUM, 0);
	}
}
}