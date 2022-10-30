#pragma once
#include "GamePlayerManager.h"
#include "OnlineManager.h"

namespace basecross
{
	class VillainPlayerObject;
	class PlayerSpawnPoint;

namespace Online
{
	class InputPlayerController;
}

	class GamePlayerManager : public Online::OnlineComponent
	{
		std::vector<std::weak_ptr<Online::InputPlayerController>> m_managedControllers;

		std::shared_ptr<VillainPlayerObject> CreatePlayerObject(int gameNumber, int playerNumber);

		std::shared_ptr<PlayerSpawnPoint> GetSpawmPoint(int uniqueId) const;

	public:
		GamePlayerManager(const std::shared_ptr<GameObject>& owner);

		void OnLateStart() override;
	};
}