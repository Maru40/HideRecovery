#pragma once
#include "PlayerObject.h"
#include "PlayerOnlineController.h"
#include "CameraHelper.h"
#include "SpringArmComponent.h"
#include "LookAtCameraManager.h"

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

		void CameraSetting()
		{
			for (auto& playerWeak : m_players)
			{
				auto player = playerWeak.lock();

				if (!player)
				{
					continue;
				}

				auto onlineController = player->GetComponent<PlayerOnlineController>();

				if (onlineController->GetPlayerNumber() == OnlineManager::GetLocalPlayer().getNumber())
				{
					auto sp = player->GetArm()->GetComponent<SpringArmComponent>();
					auto& tpsCamera = sp->GetChildObject();

					tpsCamera->AddComponent<VirtualCamera>(10);
					tpsCamera->AddComponent<LookAtCameraManager>(player, LookAtCameraManager::Parametor());
					break;
				}
			}
		}

		void OnConnected() override
		{
			OnlineManager::JoinRandomOrCreateRoom();
		}

		void OnCreateRoom() override
		{
			CameraSetting();
		}

		void OnJoinRoom() override
		{
			CameraSetting();
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