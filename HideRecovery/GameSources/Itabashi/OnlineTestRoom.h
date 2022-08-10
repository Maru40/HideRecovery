#pragma once
#include "PlayerObject.h"
#include "PlayerOnlineController.h"
#include "CameraHelper.h"
#include "SpringArmComponent.h"

namespace basecross
{
namespace Online
{
	class OnlineTester : public OnlineComponent
	{
		std::vector<std::weak_ptr<PlayerObject>> m_players;

	public:

		OnlineTester(const std::shared_ptr<GameObject>& owner) :
			OnlineComponent(owner)
		{
		}

		void AddPlayer(const std::shared_ptr<PlayerObject>& player) { m_players.push_back(player); }

		void OnConnected() override
		{
			OnlineManager::JoinRandomOrCreateRoom();
		}

		void OnCreateRoom() override
		{
			for (auto& playerWeak : m_players)
			{
				auto player = playerWeak.lock();

				if (!player)
				{
					continue;
				}

				auto onlineController = player->GetComponent<PlayerOnlineController>();

				if (onlineController->GetPlayerNumber() == OnlineManager::GetLocalPlayer().playerNumber)
				{
					auto sp = player->GetArm()->GetComponent<SpringArmComponent>();
					sp->GetChildObject()->AddComponent<VirtualCamera>(10);
					break;
				}
			}
		}

		void OnJoinRoom() override
		{
			for (auto& playerWeak : m_players)
			{
				auto player = playerWeak.lock();

				if (!player)
				{
					continue;
				}

				auto onlineController = player->GetComponent<PlayerOnlineController>();

				if (onlineController->GetPlayerNumber() == OnlineManager::GetLocalPlayer().playerNumber)
				{
					auto sp = player->GetArm()->GetComponent<SpringArmComponent>();
					sp->GetChildObject()->AddComponent<VirtualCamera>(10);
					break;
				}
			}
		}
	};


	class OnlineTestRoom : public GameObject
	{
	public:
		OnlineTestRoom(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;
	};
}
}