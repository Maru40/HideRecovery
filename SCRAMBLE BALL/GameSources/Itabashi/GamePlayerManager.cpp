#include "stdafx.h"
#include "GamePlayerManager.h"
#include "OnlineMatching.h"
#include "Itabashi/OtherPlayerObject.h"
#include "Itabashi/SelfPlayerObject.h"
#include "Maruyama/Enemy/Object/AIPlayerObject.h"

namespace basecross
{
	GamePlayerManager::GamePlayerManager(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner)
	{
		m_managedControllers.resize(Online::OnlineMatching::MAX_PLAYER_NUM);
	}

	std::shared_ptr<PlayerObject> GamePlayerManager::CreatePlayerObject(int playerNumber)
	{
		// プレイヤー番号が無効なら
		if (playerNumber == Online::OnlineManager::INVALID_ONLINE_PLAYER_NUMBER)
		{
			return nullptr;
		}

		const auto& localPlayer = Online::OnlineManager::GetLocalPlayer();

		// 自分自身なら
		if (playerNumber == localPlayer.getNumber())
		{
			return GetStage()->AddGameObject<StageObject::SelfPlayerObject>();
		}

		// AIプレイヤーであり、ホストであるなら
		if (playerNumber < 0 && localPlayer.getIsMasterClient())
		{
			return GetStage()->AddGameObject<Enemy::AIPlayerObject>();
		}

		return GetStage()->AddGameObject<StageObject::OtherPlayerObject>();
	}

	void GamePlayerManager::OnLateStart()
	{
		for (int i = 0; i < Online::OnlineMatching::MAX_PLAYER_NUM; ++i)
		{
			int playerNumber = Online::OnlineMatching::GetPlayerNumberToGameNumber(i);
			auto playerObject = CreatePlayerObject(playerNumber);
			playerObject->OnlineSetting(i, playerNumber);
		}
	}
}