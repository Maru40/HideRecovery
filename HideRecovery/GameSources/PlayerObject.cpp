
/*!
@file PlayerObject.cpp
@brief PlayerObjectクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "PlayerObject.h"

#include "PlayerMover.h"
#include "RotationController.h"
#include "PlayerController.h"

#include "CameraHelper.h"
#include "CameraRotater.h"
#include "PlayerCameraMover.h"
#include "ChasingTarget.h"
#include "SpringArmComponent.h"

#include "LookAtCameraManager.h"

#include "ItemBag.h"
#include "ItemAcquisitionManager.h"
//#include "OwnHideItemManager.h"

#include "AnimationCtrl.h"
#include "PlayerAnimationCtrl.h"
#include "VelocityManager.h"
#include "Itabashi/ObjectMover.h"
#include "Itabashi/PlayerOnlineController.h"
#include "Itabashi/OnlineTransformSynchronization.h"

#include "Watanabe/Component/PlayerAnimator.h"

#include "ChargeGun.h"
#include "Watanabe/Component/PlayerStatus.h"

#include "TackleAttack.h"
#include "CollisionAction.h"

#include "Maruyama/Player/Component/Respawner.h"
#include "Maruyama/Player/Component/PlayerDeader.h"
#include "Maruyama/Player/Component/UseWepon.h"
#include "Maruyama/Player/Component/GoalAnimationController.h"
#include "Maruyama/Player/Component/Teleport.h"
#include "Maruyama/Player/Component/FieldMap.h"

namespace basecross {

	PlayerObject::PlayerObject(const std::shared_ptr<Stage>& stage):
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
		draw->SetMultiMeshResource(L"Player_Mesh_Red");
		draw->SetMeshToTransformMatrix(spanMat);
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
		gravity->SetGravity(Vec3(0.0f ,-4.8f, 0.0f));
		
		AddComponent<ItemBag>();
		AddComponent<ItemAcquisitionManager>();
		//AddComponent<OwnHideItemManager>();

		//AddComponent<PlayerAnimationCtrl>();
		AddComponent<VelocityManager>();

		auto objecfMover = AddComponent<Operator::ObjectMover>();
		objecfMover->SetMoveSpeed(8.5f);
		//objecfMover->SetMoveSpeed(28.5f);
		objecfMover->SetAffectedCamera(GetStage()->GetView()->GetTargetCamera());
		AddComponent<Online::PlayerOnlineController>();
		AddComponent<Online::OnlineTransformSynchronization>();

		AddComponent<PlayerAnimator>();
		auto chargeGun = AddComponent<ChargeGun>();
		AddComponent<PlayerStatus>();
		AddComponent<TackleAttack>();

		AddComponent<Respawner>();
		AddComponent<PlayerDeader>();
		auto useWeapon = AddComponent<UseWepon>(chargeGun);
		AddComponent<GoalAnimationController>();
		//AddComponent<FieldMap>();
		AddComponent<Teleport>();

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