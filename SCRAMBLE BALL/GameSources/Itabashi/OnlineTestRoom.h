#pragma once
#include "PlayerObject.h"
#include "PlayerOnlineController.h"
#include "Patch/CameraHelper.h"
#include "Patch/SpringArmComponent.h"
#include "Maruyama/Camera/Component/LookAtCameraManager.h"

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
	};


	class OnlineTestRoom : public GameObject
	{
	public:
		OnlineTestRoom(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;
	};
}
}