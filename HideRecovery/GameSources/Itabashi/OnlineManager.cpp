﻿#include "stdafx.h"
#include "OnlineManager.h"

namespace basecross
{
namespace Online
{
	OnlineComponent::OnlineComponent(const std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{
		OnlineManager::AddCallBacks(this);
	}

	OnlineComponent::~OnlineComponent()
	{
		OnlineManager::RemoveCallBacks(this);
	}

	std::unique_ptr<OnlineManager> OnlineManager::m_instance;

	void OnlineManager::Update()
	{
		for (auto& removeCallBack : GetInstance()->m_removeCallBacks)
		{
			auto& callBacksVector = GetInstance()->m_callBacksVector;
			auto itr = callBacksVector.begin();

			while (itr != callBacksVector.end())
			{
				if ((*itr) == removeCallBack)
				{
					callBacksVector.erase(itr);
					break;
				}

				++itr;
			}
		}

		GetInstance()->m_removeCallBacks.clear();

		for (auto& addCallBack : GetInstance()->m_addCallBacks)
		{
			GetInstance()->m_callBacksVector.push_back(addCallBack);
		}

		GetInstance()->m_addCallBacks.clear();

		auto& client = GetInstance()->m_client;

		if (client)
		{
			client->service();
		}
	}

	void OnlineManager::AddCallBacks(I_OnlineCallBacks* callbacks)
	{
		auto& callBacksVector = GetInstance()->m_callBacksVector;
		auto result = std::find(callBacksVector.begin(), callBacksVector.end(), callbacks);

		if (result == callBacksVector.end())
		{
			GetInstance()->m_addCallBacks.push_back(callbacks);
		}
	}

	void OnlineManager::RemoveCallBacks(I_OnlineCallBacks* callbacks)
	{
		GetInstance()->m_removeCallBacks.push_back(callbacks);
	}

	const std::unique_ptr<OnlineManager>& OnlineManager::GetInstance()
	{
		if (m_instance)
		{
			return m_instance;
		}

		m_instance = std::unique_ptr<OnlineManager>(new OnlineManager());

		return m_instance;
	}

	bool OnlineManager::Connect(const std::wstring& version)
	{
		auto& client = GetInstance()->m_client;

		if (!GetInstance()->m_client)
		{
			client = std::make_unique<ExitGames::LoadBalancing::Client>(
				*GetInstance(),
				GetApplicationId().c_str(),
				version.c_str(),
				ExitGames::Photon::ConnectionProtocol::DEFAULT,
				false,
				ExitGames::LoadBalancing::RegionSelectionMode::BEST
				);
		}

		return client->connect();
	}

	void OnlineManager::Disconnect()
	{
		auto& client = GetInstance()->m_client;

		if (client)
		{
			client->disconnect();
			client.reset();
		}
	}

	void OnlineManager::CreateRoom(const std::wstring& roomName, const ExitGames::LoadBalancing::RoomOptions& roomOptions)
	{
		GetInstance()->m_client->opCreateRoom(roomName.c_str(), roomOptions);
	}

	void OnlineManager::JoinRoom(const std::wstring& roomName)
	{
		GetInstance()->m_client->opJoinRoom(roomName.c_str());
	}

	void OnlineManager::JoinRandomOrCreateRoom(const ExitGames::LoadBalancing::RoomOptions& roomOptions)
	{
		GetInstance()->m_client->opJoinRandomRoom();
	}

	void OnlineManager::RaiseEvent(bool reliable, const std::uint8_t* bytes, int size, std::uint8_t eventCode, const ExitGames::LoadBalancing::RaiseEventOptions& options)
	{
		GetInstance()->m_client->opRaiseEvent(reliable, bytes, size, eventCode, options);
	}

	void OnlineManager::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player)
	{
		std::vector<int> playerNumbers;

		for (int i = 0; i < playernrs.getSize(); ++i)
		{
			playerNumbers.push_back(playernrs.getElementAt(i));
		}

		for (auto& callBacks : m_callBacksVector)
		{
			callBacks->OnJoinRoomEventAction(playerNr, playerNumbers, player);
		}
	}

	void OnlineManager::leaveRoomEventAction(int playerNr, bool isInactive)
	{
		for (auto& callback : m_callBacksVector)
		{
			callback->OnLeaveRoomEventAction(playerNr, isInactive);
		}
	}

	void OnlineManager::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
	{
		auto bytes = ExitGames::Common::ValueObject<std::uint8_t*>(eventContent);

		for (auto& callback : m_callBacksVector)
		{
			callback->OnCustomEventAction(playerNr, eventCode, bytes.getDataCopy());
		};
	}

	void OnlineManager::connectReturn(int errorCode, const ExitGames::Common::JString& errorString,
		const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster)
	{
		if (errorCode != 0)
		{
			for (auto& callback : m_callBacksVector)
			{
				callback->OnConnectFailed(errorCode);
			}

			return;
		}

		for (auto& callback : m_callBacksVector)
		{
			callback->OnConnected();
		}
	}

	void OnlineManager::disconnectReturn()
	{
		for (auto& callback : m_callBacksVector)
		{
			callback->OnDisconnected();
		}
	}

	void OnlineManager::createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& roomProperties,
		const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString)
	{
		if (errorCode)
		{
			for (auto& callback : m_callBacksVector)
			{
				callback->OnCreateRoomFailed(errorCode);
			}

			return;
		}

		for (auto& callback : m_callBacksVector)
		{
			callback->OnCreateRoom();
		}
	}

	void OnlineManager::joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& roomProperties,
		const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString)
	{
		if (errorCode)
		{
			for (auto& callback : m_callBacksVector)
			{
				callback->OnJoinRoomFailed(errorCode);
			}

			return;
		}

		for (auto& callback : m_callBacksVector)
		{
			callback->OnJoinRoom();
		}
	}

	void OnlineManager::joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& roomProperties,
		const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString)
	{
		if (errorCode)
		{
			OnlineManager::CreateRoom(L"", ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(6));
			return;
		}

		for (auto& callback : m_callBacksVector)
		{
			callback->OnJoinRoom();
		}
	}
}
}