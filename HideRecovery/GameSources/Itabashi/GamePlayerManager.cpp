﻿#include "stdafx.h"
#include "GamePlayerManager.h"
#include "OnlineMatching.h"
#include "VillainPlayerObject.h"
#include "PlayerOnlineController.h"
#include "OnlineTransformSynchronization.h"
#include "Maruyama/Player/Component/PlayerSpawnPoint.h"
#include "MaruUtility.h"
#include "Maruyama/Player/Component/OwnArea.h"
#include "Maruyama/Player/Component/Respawner.h"
#include "Maruyama/Interface/I_TeamMember.h"
#include "SpringArmComponent.h"
#include "CameraHelper.h"
#include "LookAtCameraManager.h"

namespace basecross
{
	GamePlayerManager::GamePlayerManager(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner)
	{
		m_managedControllers.resize(Online::OnlineMatching::MAX_PLAYER_NUM);
	}

	std::shared_ptr<VillainPlayerObject> GamePlayerManager::CreatePlayerObject(int gameNumber, int playerNumber)
	{
		auto playerObject = GetStage()->AddGameObject<VillainPlayerObject>();

		auto onlineController = playerObject->GetComponent<Online::PlayerOnlineController>();
		onlineController->SetPlayerNumber(playerNumber);

		auto onlineTransform = playerObject->GetComponent<Online::OnlineTransformSynchronization>();
		onlineTransform->SetPlayerNumber(playerNumber);

		std::shared_ptr<PlayerSpawnPoint> spawnPoint;

		auto areas = maru::Utility::FindComponents<OwnArea>(GetStage());

		onlineController->SetGamePlayerNumber(gameNumber);

		auto respawner = playerObject->GetComponent<Respawner>();
		spawnPoint = GetSpawmPoint(gameNumber);
		respawner->SetSpawnPoint(spawnPoint);

		//チームメンバーにチームを割り当てる。
		if (auto teamMember = playerObject->GetComponent<I_TeamMember>(false)) {
			auto team = spawnPoint->GetTeam();
			teamMember->SetTeam(team);
			for (auto& area : areas) {
				if (area->GetTeam() == team) {
					teamMember->SetOwnArea(area);
					break;
				}
			}
		}

		auto transform = playerObject->GetComponent<Transform>();
		transform->SetPosition(spawnPoint->GetWorldPosition());

		if (playerNumber != Online::OnlineManager::GetLocalPlayer().getNumber())
		{
			return playerObject;
		}

		auto sp = playerObject->GetArm()->GetComponent<SpringArmComponent>();
		auto& tpsCamera = sp->GetChildObject();

		tpsCamera->AddComponent<VirtualCamera>(10);
		tpsCamera->AddComponent<LookAtCameraManager>(playerObject, LookAtCameraManager::Parametor());

		return playerObject;
	}

	std::shared_ptr<PlayerSpawnPoint> GamePlayerManager::GetSpawmPoint(int uniqueId) const
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

	void GamePlayerManager::OnLateStart()
	{
		for (int i = 0; i < Online::OnlineMatching::MAX_PLAYER_NUM; ++i)
		{
			auto playerObject = CreatePlayerObject(i, Online::OnlineMatching::GetPlayerNumberToGameNumber(i));
		}
	}
}