#include "stdafx.h"
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
#include "Maruyama/Player/Component/Teleport.h"
#include "SpringArmComponent.h"
#include "CameraHelper.h"
#include "LookAtCameraManager.h"
#include "Maruyama/Player/Component/UseWepon.h"
#include "MainStage.h"
#include "Watanabe/UI/UIObjects.h"
#include "Watanabe/BoardPoly/HPGaugeBP.h"
#include "Maruyama/UI/Reticle.h"
#include "Maruyama/Camera/Component/CameraForwardController.h"
#include "TeleportUI.h"
#include "ToTargetMove.h"

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
			// 他プレイヤーは板ポリHPゲージをつける
			//（位置の同期はHPGaugeBP内で設定してある）
			auto playerStatus = playerObject->GetComponent<PlayerStatus>();
			GetStage()->AddGameObject<HPGaugeBP>(playerStatus);

			auto teleport = playerObject->GetComponent<Teleport>();
			auto cameraObject = GetStage()->AddGameObject<GameObject>();
			auto toTargetMove = cameraObject->AddComponent<ToTargetMove>();
			teleport->SetToTargetMove(toTargetMove);

			return playerObject;
		}

		playerObject->AddComponent<Reticle>();

		auto sp = playerObject->GetArm()->GetComponent<SpringArmComponent>();
		auto& tpsCamera = sp->GetChildObject();

		tpsCamera->AddComponent<VirtualCamera>(10);
		tpsCamera->AddComponent<LookAtCameraManager>(playerObject, LookAtCameraManager::Parametor());

		onlineController->SetCamera(GetStage()->GetView()->GetTargetCamera());
		auto teleport = playerObject->GetComponent<Teleport>();

		auto cameraObject = GetStage()->AddGameObject<GameObject>();
		auto camera = cameraObject->AddComponent<VirtualCamera>(11);
		camera->SetUpdateActive(false);
		cameraObject->AddComponent<CameraForwardController>(camera);
		auto toTargetMove = cameraObject->AddComponent<ToTargetMove>();
		teleport->SetTeleportCamera(camera);
		teleport->SetToTargetMove(toTargetMove);

		auto useWeapon = playerObject->GetComponent<UseWepon>();
		useWeapon->SetIsUseCamera(true);

		// 自身のHPゲージにステータスコンポーネントをセット
		auto mainStage = GetGameObject()->GetTypeStage<MainStage>(false);
		if (mainStage) {
			auto hpGauge = mainStage->GetUIObjectCSVBuilder()->GetUIObject<HPGaugeUI>(L"HPGauge");
			hpGauge->SetPlayerStatus(playerObject->GetComponent<PlayerStatus>());
		}

		auto soundListener = playerObject->AddComponent<SoundListener>();

		auto teamMember = playerObject->GetComponent<I_TeamMember>(false);
		auto teleportUI = GetStage()->AddGameObject<GameObject>()->AddComponent<TeleportUI>();
		if (teamMember) {
			auto trueFunc = [teleportUI]() {
				teleportUI->GetGameObject()->SetUpdateActive(true);
			};

			auto falseFunc = [teleportUI]() {
				teleportUI->GetGameObject()->SetUpdateActive(false);
			};

			teamMember->AddReactiveIsInAreaEvent(true, trueFunc);
			teamMember->AddReactiveIsInAreaEvent(false, falseFunc);
		}

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