/*!
@file PlayerObject.cpp
@brief PlayerObjectクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "PlayerObject.h"

#include "Patch/PlayerMover.h"
#include "Maruyama/Utility/Component/RotationController.h"
//#include "PlayerController.h"

#include "Patch/CameraHelper.h"
#include "Patch/CameraRotater.h"
#include "Patch/PlayerCameraMover.h"
#include "Patch/ChasingTarget.h"
#include "Patch/SpringArmComponent.h"

#include "Maruyama/Camera/Component/LookAtCameraManager.h"

#include "Maruyama/Player/Component/ItemBag.h"
#include "Maruyama/Player/Component/ItemAcquisitionManager.h"
//#include "OwnHideItemManager.h"

#include "VelocityManager.h"
#include "Itabashi/ObjectMover.h"
#include "Itabashi/PlayerOnlineController.h"
#include "Itabashi/OnlineTransformSynchronization.h"

#include "Watanabe/Component/PlayerAnimator.h"
#include "Watanabe/Effekseer/EfkEffect.h"

#include "Maruyama/Player/Component/ChargeGun.h"
#include "Watanabe/Component/PlayerStatus.h"
#include "Watanabe/Component/HoldBallEffectEmitter.h"

#include "Maruyama/Player/Component/TackleAttack.h"
#include "Maruyama/Utility/Component/CollisionAction.h"

#include "Maruyama/Player/Component/Respawner.h"
#include "Maruyama/Player/Component/PlayerDeader.h"
#include "Maruyama/Player/Component/UseWeapon.h"
#include "Maruyama/Player/Component/GoalAnimationController.h"
#include "Maruyama/Player/Component/Teleport.h"
#include "Maruyama/UI/2D/Component/FieldMap.h"

#include "Maruyama/Enemy/Component/EyeSearchRange.h"

#include "Maruyama/Player/Component/AccessHidePlace.h"
#include "Maruyama/Player/Component/HeightDestroy.h"

#include "Itabashi/PlayerControlManager.h"

namespace basecross {
	PlayerObject::PlayerObject(const std::shared_ptr<Stage>& stage) :
		GameObject(stage)
	{}

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
		AddComponent<HoldBallEffectEmitter>(itemBag);
		//AddComponent<OwnHideItemManager>();

		//AddComponent<PlayerAnimationCtrl>();
		AddComponent<VelocityManager>();

		auto objecfMover = AddComponent<Operator::ObjectMover>();
		objecfMover->SetMoveSpeed(8.5f);
		//objecfMover->SetMoveSpeed(28.5f);
		objecfMover->SetAffectedCamera(GetStage()->GetView()->GetTargetCamera());
		AddComponent<PlayerControlManager>();
		AddComponent<Online::PlayerOnlineController>();
		AddComponent<Online::OnlineTransformSynchronization>();

		AddComponent<PlayerAnimator>();
		auto chargeGun = AddComponent<ChargeGun>();
		auto soundEmitter = AddComponent<SoundEmitter>();
		auto playerStatus = AddComponent<PlayerStatus>();
		AddComponent<TackleAttack>();

		auto respawner = AddComponent<Respawner>();
		AddComponent<PlayerDeader>();
		auto useWeapon = AddComponent<UseWeapon>(chargeGun);
		AddComponent<GoalAnimationController>();
		//AddComponent<FieldMap>();
		//AddComponent<Teleport>();
		AddComponent<EyeSearchRange>();
		//AddComponent<AccessHidePlace>();

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

		// 被弾時のエフェクトを再生を登録
		playerStatus->AddFuncAddDamage(
			[&](const std::shared_ptr<PlayerStatus>& playerStatus, const DamageData& damageData) {
				auto efkComp = playerStatus->GetGameObject()->GetComponent<EfkComponent>();
				efkComp->Play(L"PlayerHit");
			}
		);
		auto shadowmap = AddComponent<Shadowmap>();
		shadowmap->SetMultiMeshResource(L"Player_Mesh");
		shadowmap->SetMeshToTransformMatrix(spanMat);

		constexpr float ScaleValue = 1.0f;
		transform->SetScale(Vec3(ScaleValue));

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
		//auto virtualCamera = tpsCamera->AddComponent<VirtualCamera>(10);
		//tpsCamera->AddComponent<LookAtCameraManager>(GetThis<GameObject>(), LookAtCameraManager::Parametor());

		springArmComponent->AddHitTag(L"Wall");
		springArmComponent->SetChildObject(tpsCamera);
	}
}