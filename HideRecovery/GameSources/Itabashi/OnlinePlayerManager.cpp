#include "stdafx.h"
#include "OnlinePlayerManager.h"
#include "PlayerObject.h"
#include "PlayerOnlineController.h"
#include "HeroPlayerObject.h"
#include "VillainPlayerObject.h"
#include "OnlineTransformSynchronization.h"
#include "SpringArmComponent.h"
#include "CameraHelper.h"
#include "LookAtCameraManager.h"

namespace basecross
{
namespace Online
{
	Col4 testCol[OnlinePlayerManager::MAX_PLAYER_NUM] =
	{
			Col4(1,0,0,1),
			Col4(0,1,0,1),
			Col4(0,0,1,1),
			Col4(1,1,0,1),
			Col4(1,0,1,1),
			Col4(0,1,1,1)
	};

	/// <summary>
	/// ヒーローとヴィランを交互に生成するデバッグ用変数
	/// </summary>
	int debugCreateCount = 0;

	OnlinePlayerManager::OnlinePlayerManager(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner)
	{
		for (auto& used : m_numberUses)
		{
			used = false;
		}
	}

	std::vector<std::shared_ptr<PlayerObject>> OnlinePlayerManager::GetPlayers()
	{
		std::vector<std::shared_ptr<PlayerObject>> playerObjects;

		auto itr = m_players.begin();

		while (itr != m_players.end())
		{
			if (itr->expired())
			{
				itr = m_players.erase(itr);
			}
			else
			{
				playerObjects.push_back(itr->lock());
				++itr;
			}
		}

		return playerObjects;
	}

	std::shared_ptr<PlayerObject> OnlinePlayerManager::CreatePlayerObject(int playerNumber, const Vec3& position)
	{
		std::shared_ptr<PlayerObject> playerObject;

		// デバッグ用 -- ヒーローとヴィランを交互に作成する
		if (debugCreateCount % 2 == 0)
		{
			playerObject = GetStage()->AddGameObject<HeroPlayerObject>();
			auto drawer = playerObject->GetComponent<PNTBoneModelDraw>();
			drawer->SetDiffuse(Col4(1, 1, 1, 1));
		}
		else
		{
			playerObject = GetStage()->AddGameObject<VillainPlayerObject>();
			auto drawer = playerObject->GetComponent<PNTBoneModelDraw>();
			drawer->SetDiffuse(Col4(1, 0, 0, 1));
		}

		++debugCreateCount;

		auto transform = playerObject->GetComponent<Transform>();
		transform->SetPosition(position);

		auto onlineController = playerObject->GetComponent<PlayerOnlineController>();
		onlineController->SetPlayerNumber(playerNumber);

		auto onlineTransform = playerObject->GetComponent<OnlineTransformSynchronization>();
		onlineTransform->SetPlayerNumber(playerNumber);

		if (playerNumber != OnlineManager::GetLocalPlayer().getNumber())
		{
			return playerObject;
		}

		auto sp = playerObject->GetArm()->GetComponent<SpringArmComponent>();
		auto& tpsCamera = sp->GetChildObject();

		tpsCamera->AddComponent<VirtualCamera>(10);
		tpsCamera->AddComponent<LookAtCameraManager>(playerObject, LookAtCameraManager::Parametor());

		return playerObject;
	}

	void OnlinePlayerManager::OnJoinRoomEventAction(int playerNumber, const std::vector<int>& playerNumbers, const ExitGames::LoadBalancing::Player& player)
	{
		auto players = GetPlayers();

		for (auto number : playerNumbers)
		{
			auto find = std::find_if(players.begin(), players.end(),
				[number](const std::shared_ptr<PlayerObject>& player) {return number == player->GetComponent<PlayerOnlineController>()->GetPlayerNumber(); });

			if (find == players.end())
			{
				auto playerObject = CreatePlayerObject(number, Vec3(20, 1, 0));
				m_players.push_back(playerObject);
			}
		}
	}
	void OnlinePlayerManager::OnLeaveRoomEventAction(int playerNumber, bool isInactive)
	{
		auto players = GetPlayers();

		auto find = std::find_if(players.begin(), players.end(),
			[playerNumber](const std::shared_ptr<PlayerObject>& player) {return playerNumber == player->GetComponent<PlayerOnlineController>()->GetPlayerNumber(); });

		if (find == players.end())
		{
			return;
		}

		auto itr = std::find_if(m_players.begin(), m_players.end(), [find](const std::weak_ptr<PlayerObject>& player) {return *find == player.lock(); });
		m_players.erase(itr);

		GetStage()->RemoveGameObject<GameObject>(*find);
	}
}
}