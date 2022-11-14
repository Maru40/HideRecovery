﻿#include "SelfPlayerObject.h"
#include "Maruyama/UI/2D/Component/Reticle.h"
#include "InputPlayerController.h"
#include "Patch/SpringArmComponent.h"
#include "Patch/CameraHelper.h"
#include "Maruyama/Camera/Component/LookAtCameraManager.h"
#include "PlayerControlManager.h"
#include "Maruyama/Camera/Component/CameraForwardController.h"
#include "Maruyama/Utility/Component/ToTargetMove.h"
#include "Maruyama/Player/Component/Teleport.h"
#include "Maruyama/Player/Component/UseWeapon.h"
#include "Maruyama/Player/Component/HidePlaceOpener.h"
#include "MainStage.h"
#include "Watanabe/UI/HPGaugeUI.h"
#include "Watanabe/UI/SimpleSprite.h"
#include "Watanabe/UI/UIObjectCSVBuilder.h"
#include "Maruyama/UI/2D/Component/TeleportUI.h"
#include "Maruyama/Utility/SingletonComponent/GameManager.h"
#include "Itabashi/ObjectMover.h"
#include "Itabashi/OnlineTransformSynchronization.h"

namespace basecross
{
namespace StageObject
{
	SelfPlayerObject::SelfPlayerObject(const std::shared_ptr<Stage>& stage) :
		PlayerObject(stage)
	{

	}

	void SelfPlayerObject::OnCreate()
	{
		PlayerObject::OnCreate();
		auto objecfMover = GetComponent<Operator::ObjectMover>();
		objecfMover->SetAffectedCamera(GetStage()->GetView()->GetTargetCamera());
	}

	void SelfPlayerObject::OnlineSetting(int gameNumber, int playerNumber)
	{
		PlayerObject::OnlineSetting(gameNumber, playerNumber);

		auto playerStatus = GetComponent<PlayerStatus>();

		AddComponent<Reticle>();
		AddComponent<InputPlayerController>();

		auto springArm = GetArm()->GetComponent<SpringArmComponent>();
		auto& tpsCamera = springArm->GetChildObject();

		tpsCamera->AddComponent<VirtualCamera>(10);
		tpsCamera->AddComponent<LookAtCameraManager>(GetThis<GameObject>(), LookAtCameraManager::Parametor());

		auto controlManager = GetComponent<PlayerControlManager>();
		controlManager->SetCamera(GetStage()->GetView()->GetTargetCamera());

		auto teleport = GetComponent<Teleport>();

		auto cameraObject = GetStage()->AddGameObject<GameObject>();
		auto camera = cameraObject->AddComponent<VirtualCamera>(11);
		camera->SetUpdateActive(false);
		cameraObject->AddComponent<CameraForwardController>(camera);
		auto toTargetMove = cameraObject->AddComponent<ToTargetMove>();
		teleport->SetTeleportCamera(camera);
		teleport->SetToTargetMove(toTargetMove);

		auto useWeapon = GetComponent<UseWeapon>();
		useWeapon->SetIsUseCamera(true);

		auto accessHidePlace = GetComponent<HidePlaceOpener>();
		accessHidePlace->SetIsDrawUI(true);

		// 自身のHPゲージにステータスコンポーネントをセット
		auto mainStage = GetTypeStage<MainStage>(false);
		if (mainStage) {
			auto hpGauge = mainStage->GetUIObjectCSVBuilder()->GetUIObject<HPGaugeUI>(L"HPGauge");
			hpGauge->SetPlayerStatus(GetComponent<PlayerStatus>());

			auto playerLabel = mainStage->GetUIObjectCSVBuilder()->GetUIObject<SimpleSprite>(L"PlayerLabel");
			auto playerNumber = mainStage->GetUIObjectCSVBuilder()->GetUIObject<Numbers>(L"PlayerNumber");
			Col4 teamColor = team::GetTeamColor(playerStatus->GetTeam());
			playerLabel->GetDrawComponent()->SetDiffuse(teamColor);
			playerNumber->SetColor(teamColor);

			playerLabel->ChangeSprite(SimpleSprite::Type::SpriteData, team::GetTeamTypeString(playerStatus->GetTeam()));
			playerNumber->SetNumber((gameNumber % 3) + 1);
		}

		auto soundListener = AddComponent<SoundListener>();

		auto teamMember = GetComponent<I_TeamMember>(false);
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

		auto transformSynchronizer = GetComponent<Online::OnlineTransformSynchronization>();
		transformSynchronizer->SetIsMaster(true);
	}
}
}