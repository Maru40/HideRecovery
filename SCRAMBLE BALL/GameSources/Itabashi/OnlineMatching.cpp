#include "stdafx.h"
#include "OnlineMatching.h"
#include <iomanip>
#include "Watanabe/UI/Numbers.h"
#include "Watanabe/Component/MatchingUIController.h"

namespace basecross
{
namespace Online
{
	std::mt19937_64 m_randomEngine = std::mt19937_64(std::random_device()());

	int OnlineMatching::m_playerNumbers[OnlineMatching::MAX_PLAYER_NUM] = {};

	OnlineMatching::OnlineMatching(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner),
		m_passwordDistribution(0, PASSWORD_UINT_MAX)
	{
		m_roomOptions.setMaxPlayers(MAX_PLAYER_NUM);
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

	std::wstring OnlineMatching::CreatePassword()
	{
		std::wstringstream wss;
		wss << std::setfill(L'0') << std::setw(PASSWORD_DISIT_NUM) << m_passwordDistribution(m_randomEngine);

		return wss.str();
	}

	void OnlineMatching::OnCreate()
	{
		Online::OnlineManager::Connect();
	}

	void OnlineMatching::OnLateStart()
	{
	}

	void OnlineMatching::OnConnected()
	{
		m_isConnected = true;
	}

	void OnlineMatching::OnCreateRoom()
	{
	}

	void OnlineMatching::OnCreateRoomFailed(int errorCode)
	{
		if (!m_usePassword)
		{
			return;
		}

		OnlineManager::CreateRoom(CreatePassword(), m_roomOptions);
	}

	void OnlineMatching::OnJoinRoom()
	{
	}

	void OnlineMatching::OnJoinRoomFailed(int errorCode)
	{
		m_isMatching = false;
	}

	void OnlineMatching::OnLeaveRoom()
	{
		m_isMatching = false;
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

		// AI用番号割振り
		int aiPlayerNumber = -1;

		for (auto& playerNumber : m_playerNumbers)
		{
			if (playerNumber == OnlineManager::INVALID_ONLINE_PLAYER_NUMBER)
			{
				playerNumber = aiPlayerNumber;
				--aiPlayerNumber;
			}
		}

		OnlineManager::RaiseEvent(false, (std::uint8_t*)m_playerNumbers, sizeof(m_playerNumbers), EXECUTE_UPDATE_PLAYER_NUMBERS);
	}

	void OnlineMatching::StartFreeMatching()
	{
		if (!m_isConnected || m_isMatching)
		{
			return;
		}

		m_isMatching = true;
		m_usePassword = false;

		m_matchingUIController.lock()->ChangeUIStartMatching();

		m_roomOptions.setIsVisible(true);
		Online::OnlineManager::JoinRandomOrCreateRoom(m_roomOptions);
	}

	void OnlineMatching::StartCreatePasswordMatching()
	{
		if (!m_isConnected || m_isMatching)
		{
			return;
		}

		m_isMatching = true;
		m_usePassword = true;

		m_matchingUIController.lock()->ChangeUIStartMatching();

		m_roomOptions.setIsVisible(false);
		Online::OnlineManager::CreateRoom(CreatePassword(), m_roomOptions);
	}

	void OnlineMatching::StartJoinPasswordMatching(const std::wstring& password)
	{
		if (!m_isConnected || m_isMatching)
		{
			return;
		}

		m_isMatching = true;
		m_usePassword = true;

		m_matchingUIController.lock()->ChangeUIStartMatching();

		OnlineManager::JoinRoom(password);
	}

	void OnlineMatching::StartLeaveRoom()
	{
		OnlineManager::LeaveRoom();
	}

}
}