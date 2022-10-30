#include "stdafx.h"
#include "GamePlayerManager.h"
#include "OnlineMatching.h"
#include "Maruyama/Player/Object/VillainPlayerObject.h"
#include "InputPlayerController.h"
#include "PlayerControlManager.h"
#include "OnlineTransformSynchronization.h"
#include "Maruyama/Player/Component/PlayerSpawnPoint.h"
#include "Maruyama/Utility/Utility.h"
#include "Maruyama/Player/Component/OwnArea.h"
#include "Maruyama/Player/Component/Respawner.h"
#include "Maruyama/Interface/I_TeamMember.h"
#include "Maruyama/Player/Component/Teleport.h"
#include "Patch/SpringArmComponent.h"
#include "Patch/CameraHelper.h"
#include "Maruyama/Camera/Component/LookAtCameraManager.h"
#include "Maruyama/Player/Component/UseWeapon.h"
#include "MainStage.h"
#include "Watanabe/UI/UIObjects.h"
#include "Watanabe/BoardPoly/HPGaugeBP.h"
#include "Watanabe/BoardPoly/PlayerLabelBP.h"
#include "Watanabe/Component/PlayerStatus.h"
#include "Watanabe/Component/SyncObject.h"
#include "Maruyama/UI/2D/Component/Reticle.h"
#include "Maruyama/Camera/Component/CameraForwardController.h"
#include "Maruyama/UI/2D/Component/TeleportUI.h"
#include "Maruyama/Utility/Component/ToTargetMove.h"
#include "Maruyama/Utility/SingletonComponent/GameManager.h"
#include "Maruyama/Player/Component/AccessHidePlace.h"
#include "OnlinePlayerSynchronizer.h"

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

		auto onlinePlayerSynchronizer = playerObject->GetComponent<OnlinePlayerSynchronizer>();
		onlinePlayerSynchronizer->SetOnlinePlayerNumber(playerNumber);
		onlinePlayerSynchronizer->SetGamePlayerNumber(gameNumber);

		auto onlineTransform = playerObject->GetComponent<Online::OnlineTransformSynchronization>();
		onlineTransform->SetOnlinePlayerNumber(playerNumber);

		std::shared_ptr<PlayerSpawnPoint> spawnPoint;

		auto areas = maru::Utility::FindComponents<OwnArea>(GetStage());

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
		transform->SetQuaternion(spawnPoint->GetQuaternion());

		auto playerStatus = playerObject->GetComponent<PlayerStatus>();
		auto playerTeam = playerStatus->GetTeam();

		if (playerNumber != Online::OnlineManager::GetLocalPlayer().getNumber())
		{
			// 他プレイヤーは板ポリHPゲージをつける
			//（位置の同期はHPGaugeBP内で設定してある）
			GetStage()->AddGameObject<HPGaugeBP>(playerStatus);
			// (playerNumber % 3) + 1で1～6が 1,2,3 1,2,3になる
			auto label = GetStage()->AddGameObject<PlayerLabelBP>(
				team::GetTeamTypeString(playerTeam), (gameNumber % 3) + 1);

			auto syncComp = label->AddComponent<SyncObject>();
			syncComp->SetTarget(transform);
			syncComp->SetOffsetPosition(Vec3(0, 1.7f, 0));

			auto teleport = playerObject->GetComponent<Teleport>();
			auto cameraObject = GetStage()->AddGameObject<GameObject>();
			auto toTargetMove = cameraObject->AddComponent<ToTargetMove>();
			teleport->SetToTargetMove(toTargetMove);

			return playerObject;
		}

		playerObject->AddComponent<Reticle>();
		playerObject->AddComponent<InputPlayerController>();

		auto springArm = playerObject->GetArm()->GetComponent<SpringArmComponent>();
		auto& tpsCamera = springArm->GetChildObject();

		tpsCamera->AddComponent<VirtualCamera>(10);
		tpsCamera->AddComponent<LookAtCameraManager>(playerObject, LookAtCameraManager::Parametor());

		auto controlManager = playerObject->GetComponent<PlayerControlManager>();
		controlManager->SetCamera(GetStage()->GetView()->GetTargetCamera());

		auto teleport = playerObject->GetComponent<Teleport>();

		auto cameraObject = GetStage()->AddGameObject<GameObject>();
		auto camera = cameraObject->AddComponent<VirtualCamera>(11);
		camera->SetUpdateActive(false);
		cameraObject->AddComponent<CameraForwardController>(camera);
		auto toTargetMove = cameraObject->AddComponent<ToTargetMove>();
		teleport->SetTeleportCamera(camera);
		teleport->SetToTargetMove(toTargetMove);

		auto useWeapon = playerObject->GetComponent<UseWeapon>();
		useWeapon->SetIsUseCamera(true);

		auto accessHidePlace = playerObject->GetComponent<HidePlaceOpener>();
		accessHidePlace->SetIsDrawUI(true);

		// 自身のHPゲージにステータスコンポーネントをセット
		auto mainStage = GetGameObject()->GetTypeStage<MainStage>(false);
		if (mainStage) {
			auto hpGauge = mainStage->GetUIObjectCSVBuilder()->GetUIObject<HPGaugeUI>(L"HPGauge");
			hpGauge->SetPlayerStatus(playerObject->GetComponent<PlayerStatus>());

			auto playerLabel = mainStage->GetUIObjectCSVBuilder()->GetUIObject<SimpleSprite>(L"PlayerLabel");
			auto playerNumber = mainStage->GetUIObjectCSVBuilder()->GetUIObject<Numbers>(L"PlayerNumber");
			Col4 teamColor = team::GetTeamColor(playerStatus->GetTeam());
			playerLabel->GetDrawComponent()->SetDiffuse(teamColor);
			playerNumber->SetColor(teamColor);

			playerLabel->ChangeSprite(SimpleSprite::Type::SpriteData, team::GetTeamTypeString(playerStatus->GetTeam()));
			playerNumber->SetNumber((gameNumber % 3) + 1);
		}

		auto soundListener = playerObject->AddComponent<SoundListener>();

		auto teamMember = playerObject->GetComponent<I_TeamMember>(false);
		auto teleportUI = GetStage()->AddGameObject<GameObject>()->AddComponent<TeleportUI>();
		if (teamMember) {
			auto trueFunc = [teleportUI]() {
				auto gameManager = GameManager::GetInstance();
				if (gameManager && gameManager->GetCurrentState() != GameManager::State::Game) {
					return;
				}

				teleportUI->GetGameObject()->SetUpdateActive(true);
			};

			auto falseFunc = [teleportUI]() {
				teleportUI->GetGameObject()->SetUpdateActive(false);
			};

			teamMember->AddReactiveIsInAreaEvent(true, trueFunc);
			teamMember->AddReactiveIsInAreaEvent(false, falseFunc);
		}

		if (springArm && teamMember) {
			auto rad = springArm->GetRadXZ();
			auto degree = teamMember->GetTeam() == team::TeamType::Blue ? -90.0f : 90.0f;
			rad = XMConvertToRadians(degree);
			springArm->SetRadXZ(rad);
			springArm->OnUpdate2();
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