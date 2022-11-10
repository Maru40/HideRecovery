#include "stdafx.h"
#include "GamePlayerManager.h"
#include "OnlineMatching.h"
#include "Itabashi/OtherPlayerObject.h"
#include "Itabashi/SelfPlayerObject.h"

namespace basecross
{
	GamePlayerManager::GamePlayerManager(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner)
	{
		m_managedControllers.resize(Online::OnlineMatching::MAX_PLAYER_NUM);
	}

	std::shared_ptr<PlayerObject> GamePlayerManager::CreatePlayerObject(int gameNumber, int playerNumber)
	{
		std::shared_ptr<PlayerObject> playerObject;

		// 自分自身なら
		if (playerNumber == Online::OnlineManager::GetLocalPlayer().getNumber())
		{
			playerObject = GetStage()->AddGameObject<StageObject::SelfPlayerObject>();
		}
		else
		{
			playerObject = GetStage()->AddGameObject<StageObject::OtherPlayerObject>();
		}

		playerObject->OnlineSetting(gameNumber, playerNumber);

		return playerObject;
	}

	void GamePlayerManager::OnLateStart()
	{
		for (int i = 0; i < Online::OnlineMatching::MAX_PLAYER_NUM; ++i)
		{
			auto playerObject = CreatePlayerObject(i, Online::OnlineMatching::GetPlayerNumberToGameNumber(i));
		}
	}
}