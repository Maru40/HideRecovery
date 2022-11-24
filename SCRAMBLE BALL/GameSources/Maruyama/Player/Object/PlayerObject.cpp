﻿/*!
@file PlayerObject.cpp
@brief PlayerObjectクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "PlayerObject.h"

#include "Patch/PlayerMover.h"
#include "Maruyama/Utility/Component/RotationController.h"

#include "Patch/CameraHelper.h"
#include "Patch/CameraRotater.h"
#include "Patch/PlayerCameraMover.h"
#include "Patch/ChasingTarget.h"
#include "Patch/SpringArmComponent.h"

#include "Maruyama/Camera/Component/LookAtCameraManager.h"

#include "Maruyama/Player/Component/ItemBag.h"
#include "Maruyama/Player/Component/ItemAcquisitionManager.h"

#include "VelocityManager.h"
#include "Itabashi/ObjectMover.h"
#include "Itabashi/OnlinePlayerSynchronizer.h"

#include "Watanabe/Component/PlayerAnimator.h"
#include "Watanabe/Effekseer/EfkEffect.h"

#include "Maruyama/Player/Component/ChargeGun.h"
#include "Watanabe/Component/PlayerStatus.h"
#include "Watanabe/Component/HasBallEventExecuter.h"
#include "Watanabe/Component/DissolveAnimator.h"
#include "Watanabe/UI/UIObjectCSVBuilder.h"
#include "Watanabe/UI/DirectionWithHasBallUI.h"

#include "MainStage.h"

#include "Maruyama/Player/Component/TackleAttack.h"
#include "Maruyama/Utility/Component/CollisionAction.h"

#include "Maruyama/Player/Component/Respawner.h"
#include "Maruyama/Player/Component/PlayerDeader.h"
#include "Maruyama/Player/Component/UseWeapon.h"
#include "Maruyama/Player/Component/GoalAnimationController.h"
#include "Maruyama/Player/Component/Teleport.h"
#include "Maruyama/UI/2D/Component/FieldMap.h"

#include "Maruyama/Enemy/Component/EyeSearchRange.h"

#include "Maruyama/Player/Component/HidePlaceOpener.h"
#include "Maruyama/Player/Component/HeightDestroy.h"

#include "Itabashi/PlayerControlManager.h"

#include "Maruyama/Utility/Utility.h"
#include "Maruyama/Player/Component/OwnArea.h"
#include "Maruyama/Player/Component/PlayerSpawnPoint.h"
#include "Itabashi/OnlineTransformSynchronization.h"
#include "Itabashi/OnlineMatching.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"

// AI操作用

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Utility/Component/SeekTarget.h"
#include "Maruyama/Enemy/Component/SelfAstarNodeController.h"
#include "Maruyama/Utility/Component/TargetManager.h"
#include "Maruyama/Enemy/Component/Stator/AIPlayerStator.h"
#include "Maruyama/Enemy/Component/AIVirtualController.h"
#include "Maruyama/Enemy/Component/WallAvoid.h"

#include "Maruyama/Utility/Component/Targeted.h"

#include "Itabashi/AIActiveChanger.h"

namespace basecross {
	PlayerObject::PlayerObject(const std::shared_ptr<Stage>& stage) :
		GameObject(stage)
	{}

	std::shared_ptr<PlayerSpawnPoint> PlayerObject::GetSpawmPoint(int uniqueId) const
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

	void PlayerObject::OnCreate() {
		Mat4x4 spanMat;
		const float fScale = 0.8f;
		Vec3 scale = Vec3(fScale);
		spanMat.affineTransformation(
			scale,
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		auto draw = AddComponent<DrawComp>();
		draw->SetMultiMeshResource(L"Player_Mesh");
		draw->SetMeshToTransformMatrix(spanMat);
		draw->SetModelTextureEnabled(false);

		// アウトライン関係
		draw->SetOutlineActive(true);
		// ディゾブルアニメーションを有効
		draw->SetEnabledDissolve(true);

		SetAlphaActive(true);
		//draw->SetDiffuse(Col4(0.0f, 0.0f, 1.0f, 1.0f));

		auto collision = AddComponent<CollisionObb>();
		//collision->SetDrawActive(true);
		constexpr float CollisionSize = 1.0f;
		collision->SetMakedSize(CollisionSize);
		AddComponent<maru::CollisionAction>();
		AddComponent<RotationController>();
		//AddComponent<PlayerController>();
		auto gravity = AddComponent<Gravity>();
		gravity->SetGravity(Vec3(0.0f, -9.8f * 3.0f, 0.0f));

		auto itemBag = AddComponent<ItemBag>();
		AddComponent<ItemAcquisitionManager>();

		AddComponent<VelocityManager>();

		auto objectMover = AddComponent<Operator::ObjectMover>();
		objectMover->SetMoveSpeed(8.5f);
		AddComponent<PlayerControlManager>();
		AddComponent<OnlinePlayerSynchronizer>();

		std::weak_ptr<DissolveAnimator> weakDissolveAnimator = AddComponent<DissolveAnimator>();
		auto animator = AddComponent<PlayerAnimator>();
		// 死んだらディゾブル開始
		animator->AddAnimationEvent(PlayerAnimationState::State::Dead,
			[weakDissolveAnimator]() {
				weakDissolveAnimator.lock()->Start();
			},
			nullptr, nullptr
				);
		animator->AddAnimationEvent(PlayerAnimationState::State::GSDead,
			[weakDissolveAnimator]() {
				weakDissolveAnimator.lock()->Start();
			},
			nullptr, nullptr
				);

		auto chargeGun = AddComponent<ChargeGun>();
		auto soundEmitter = AddComponent<SoundEmitter>();
		auto playerStatus = AddComponent<PlayerStatus>();

		auto respawner = AddComponent<Respawner>();
		AddComponent<PlayerDeader>();
		auto useWeapon = AddComponent<UseWeapon>(chargeGun);
		AddComponent<GoalAnimationController>();
		AddComponent<EyeSearchRange>();
		AddComponent<HidePlaceOpener>();

		auto transformsynchronizer = AddComponent<Online::OnlineTransformSynchronization>();
		transformsynchronizer->SetIsSynchronizeRotation(false);

		AddComponent<Teleport>();	//テレポートの生成
		auto heightDestory = AddComponent<HeightDestroy>();
		std::weak_ptr<Respawner> weakRespawner = respawner;
		heightDestory->AddDestroyAction([weakRespawner]() { weakRespawner.lock()->StartRespawn(); });

		// エフェクトの設定
		auto efkComp = AddComponent<EfkComponent>();
		efkComp->SetEffectResource(L"Respawn", TransformData(Vec3(0, -0.5f, 0), Vec3(0.3f, 2.0f, 0.3f)));
		efkComp->SetEffectResource(L"Smoke", TransformData(), true);
		efkComp->SetEffectResource(L"PlayerHit", TransformData(Vec3(0, 0.5f, 0), Vec3(0.5f)), true);
		efkComp->IsSyncGameObject(L"PlayerHit", true);
		efkComp->SetEffectResource(L"MuzzleFlash", TransformData(Vec3(0, 0.5f, 0.5f), Vec3(0.5f), Vec3(0, 180, 0)), true);
		efkComp->IsSyncGameObject(L"MuzzleFlash", true);
		efkComp->SetEffectResource(L"Tackle");
		efkComp->IsSyncGameObject(L"Tackle", true);
		efkComp->SetEffectResource(L"HasBall", TransformData(Vec3(0, 0.5f, 0), Vec3(0.5f)));
		efkComp->IsSyncGameObject(L"HasBall", true);
		efkComp->SetEffectResource(L"Excl", TransformData(Vec3(0, 1.7f, 0)));
		efkComp->IsSyncGameObject(L"Excl", true);

		// 被弾時のエフェクトを再生を登録
		playerStatus->AddFuncAddDamage(
			[&](const std::shared_ptr<PlayerStatus>& playerStatus, const DamageData& damageData) {
				auto efkComp = playerStatus->GetGameObject()->GetComponent<EfkComponent>();
				efkComp->Play(L"PlayerHit");
			}
		);

		// ボールを持ったとき & 持っていないときのイベント
		auto hasBallEvent = AddComponent<HasBallEventExecuter>(itemBag);
		// エフェクト
		hasBallEvent->AddEvent(
			[](const shared_ptr<PlayerObject>& owner) {
				auto efkComp = owner->GetComponent<EfkComponent>();
				// ボールを持っている人を強調
				if (!efkComp->IsPlaying(L"HasBall")) {
					efkComp->PlayLoop(L"HasBall");
				}
			},
			[](const shared_ptr<PlayerObject>& owner) {
				auto efkComp = owner->GetComponent<EfkComponent>();
				efkComp->Stop(L"HasBall");
			}
			);

		m_directionUI = GetTypeStage<GameStageBase>()->GetUIObjectCSVBuilder()
			->GetUIObject<DirectionWithHasBallUI>(L"DirectionWithHasBallUI");

		// 自分がボールを持ったときに方向を知らせるUIに情報を渡す
		hasBallEvent->AddHaveBallEvent(
			[](const shared_ptr<PlayerObject>& owner) {
				if (auto directionUI = owner->GetDirectionWithHasBallUI())
					directionUI->SetTarget(owner);
			}
		);

		auto shadowmap = AddComponent<Shadowmap>();
		shadowmap->SetMultiMeshResource(L"Player_Mesh");
		shadowmap->SetMeshToTransformMatrix(spanMat);

		constexpr float ScaleValue = 1.0f;
		transform->SetScale(Vec3(ScaleValue));

		//ターゲット判定の追加
		auto targeted = AddComponent<Targeted>(Targeted::Parametor(TargetedPriority::PLAYER));
		std::weak_ptr<PlayerStatus> weakPlayerStatus = playerStatus;
		//死亡中はターゲットにできないように変更。
		targeted->AddCanTargetFunction([weakPlayerStatus]() { return !weakPlayerStatus.lock()->IsDead(); });

		AddTag(L"T_Player");

		//カメラセッティング----------------------------------------------------------

		auto springArm = GetStage()->Instantiate<GameObject>(Vec3(), Quat());

		auto chasingTarget = springArm->AddComponent<ChasingTarget>();
		chasingTarget->SetTarget(GetThis<GameObject>());
		auto cameraMover = springArm->AddComponent<PlayerCameraMover>();
		cameraMover->SetMinRotX(-XM_PIDIV4);
		cameraMover->SetMaxRotX(XM_PIDIV4);
		auto springArmComponent = springArm->AddComponent<SpringArmComponent>();
		springArmComponent->SetPlayer(GetThis<GameObject>());

		m_arm = springArm;

		Quat quat = Quat::Identity();
		quat.rotationY(XM_PI);

		auto tpsCamera = GetStage()->Instantiate<GameObject>(Vec3(0, 0, 3), quat);

		springArmComponent->AddHitTag(L"Wall");
		springArmComponent->SetChildObject(tpsCamera);

		// AI用コンポーネント

		AddComponent<Enemy::EnemyBase>()->SetUpdateActive(false);
		AddComponent<SeekTarget>(nullptr)->SetUpdateActive(false);
		AddComponent<SelfAstarNodeController>();
		AddComponent<TargetManager>()->SetUpdateActive(false);
		AddComponent<WallAvoid>()->SetUpdateActive(false);
		AddComponent<AIVirtualController>()->SetUpdateActive(false);
		AddComponent<Enemy::AIPlayerStator>()->SetUpdateActive(false);

		AddComponent<AIActiveChanger>();

		if (auto shareClass = ShareClassesManager::GetInstance()) {
			shareClass->AddShareClass<PlayerObject>(GetThis<PlayerObject>());
		}
	}

	void PlayerObject::OnlineSetting(int gameNumber, int playerNumber)
	{
		auto onlinePlayerSynchronizer = GetComponent<OnlinePlayerSynchronizer>();
		onlinePlayerSynchronizer->SetOnlinePlayerNumber(playerNumber);
		onlinePlayerSynchronizer->SetGamePlayerNumber(gameNumber);

		auto areas = maru::Utility::FindComponents<OwnArea>(GetStage());

		auto respawner = GetComponent<Respawner>();
		auto spawnPoint = GetSpawmPoint(gameNumber);
		respawner->SetSpawnPoint(spawnPoint);

		//チームメンバーにチームを割り当てる。
		if (auto teamMember = GetComponent<I_TeamMember>(false)) {
			auto team = spawnPoint->GetTeam();
			teamMember->SetTeam(team);
			for (auto& area : areas) {
				if (area->GetTeam() == team) {
					teamMember->SetOwnArea(area);
					break;
				}
			}
		}

		auto transform = GetComponent<Transform>();
		transform->SetPosition(spawnPoint->GetWorldPosition());
		transform->SetQuaternion(spawnPoint->GetQuaternion());

		auto transformSynchronizer = GetComponent<Online::OnlineTransformSynchronization>();
		transformSynchronizer->SetOnlinePlayerNumber(playerNumber);

		transformSynchronizer->SetCountSpan(1.0f / Online::OnlineMatching::MAX_PLAYER_NUM * gameNumber);
	}
}