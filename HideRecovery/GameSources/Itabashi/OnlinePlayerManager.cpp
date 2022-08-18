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
#include "Maruyama/Player/Component/PlayerSpawnPoint.h"
#include "Maruyama/Player/Component/Respawner.h"

namespace basecross
{
namespace Online
{
	std::shared_ptr<PlayerSpawnPoint> OnlinePlayerManager::GetSpawmPoint(int uniqueId) const
	{
		for (auto& gameObject : GetStage()->GetGameObjectVec())
		{
			auto playerSpawnPoint = gameObject->GetComponent<PlayerSpawnPoint>(false);
			
			if (playerSpawnPoint && playerSpawnPoint->GetID() == uniqueId)
			{
				return playerSpawnPoint;
			}
		}

		return nullptr;
	}

	OnlinePlayerManager::OnlinePlayerManager(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner),
		m_numberUses()
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

	std::shared_ptr<PlayerObject> OnlinePlayerManager::CreatePlayerObject(int playerNumber)
	{
		std::shared_ptr<PlayerObject> playerObject = GetStage()->AddGameObject<VillainPlayerObject>();

		auto onlineController = playerObject->GetComponent<PlayerOnlineController>();
		onlineController->SetPlayerNumber(playerNumber);

		auto onlineTransform = playerObject->GetComponent<OnlineTransformSynchronization>();
		onlineTransform->SetPlayerNumber(playerNumber);

		std::shared_ptr<PlayerSpawnPoint> spawnPoint;

		for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		{
			if (!m_numberUses[i])
			{
				onlineController->SetGamePlayerNumber(i);

				auto respawner = playerObject->GetComponent<Respawner>();
				spawnPoint = GetSpawmPoint(i);
				respawner->SetSpawnPoint(spawnPoint);

				m_numberUses[i] = true;
				break;
			}
		}

		auto transform = playerObject->GetComponent<Transform>();
		transform->SetPosition(spawnPoint->GetWorldPosition());

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
				auto playerObject = CreatePlayerObject(number);
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

		auto onlineController = (*find)->GetComponent<PlayerOnlineController>();
		m_numberUses[onlineController->GetGamePlayerNumber()] = false;
		GetStage()->RemoveGameObject<GameObject>(*find);
	}
}
}