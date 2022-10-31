#include "stdafx.h"
#include "OnlineMatching.h"
#include <random>

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
			if (isAdd) // 追加なら
			{
				if (number == 0)
				{
					number = playerNumber;
					break;
				}
			}
			else // 削除なら
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

	void OnlineMatching::OnCreate()
	{
		Online::OnlineManager::Connect();
	}

	void OnlineMatching::OnConnected()
	{
		Online::OnlineManager::JoinRandomOrCreateRoom(ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(MAX_PLAYER_NUM));
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
		std::fill_n(m_playerNumbers, MAX_PLAYER_NUM, OnlineManager::INVALID_ONLINE_PLAYER_NUMBER);
	}

	void OnlineMatching::ShuffleTeam()
	{
		int count = GetPlayerCount();
		
		int team1Count = count / 2;
		int team2Count = count - team1Count;

		std::random_device rd;
		std::default_random_engine eng(rd());

		if (eng() % 2 == 0)
		{
			std::swap(team1Count, team2Count);
		}

		std::vector<int> gameNumbers;
		Reset();

		for (int i = 0; i < team1Count; ++i)
		{
			gameNumbers.push_back(i);
		}

		for (int i = 0; i < team2Count; ++i)
		{
			gameNumbers.push_back(i + 3);
		}

		auto& players = OnlineManager::GetCurrentlyJoinedRoom().getPlayers();

		for (unsigned int i = 0; i < players.getSize(); ++i)
		{
			unsigned int index = eng() % gameNumbers.size();

			m_playerNumbers[gameNumbers[index]] = players.getElementAt(i)->getNumber();

			gameNumbers.erase(gameNumbers.begin() + index);
		}

		OnlineManager::RaiseEvent(false, (std::uint8_t*)m_playerNumbers, sizeof(m_playerNumbers), EXECUTE_UPDATE_PLAYER_NUMBERS);
	}

}
}