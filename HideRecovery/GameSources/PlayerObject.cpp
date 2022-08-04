
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

namespace basecross {

	PlayerObject::PlayerObject(const std::shared_ptr<Stage>& stage):
		GameObject(stage)
	{}

	void PlayerObject::OnCreate() {
		//デバッグ
		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");
		//draw->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 0.3f));
		SetAlphaActive(true);

		transform->SetScale(Vec3(1.0f, 1.5f, 1.0f));

		AddComponent<CollisionObb>();
		auto playerMover = AddComponent<PlayerMover>();
		playerMover->SetIsCameraAffected(true);
		AddComponent<RotationController>();
		AddComponent<PlayerController>();
		//AddComponent<Gravity>();

		auto springArm = GetStage()->Instantiate<GameObject>(Vec3(), Quat());

		auto chasingTarget = springArm->AddComponent<ChasingTarget>();
		chasingTarget->SetTarget(GetThis<GameObject>());
		auto cameraMover = springArm->AddComponent<PlayerCameraMover>();
		cameraMover->SetMinRotX(-XM_PIDIV4);
		cameraMover->SetMaxRotX(XM_PIDIV4);
		auto springArmComponent = springArm->AddComponent<SpringArmComponent>();

		Quat quat = Quat::Identity();
		quat.rotationY(XM_PI);

		auto tpsCamera = GetStage()->Instantiate<GameObject>(Vec3(0, 0, 3), quat);
		auto virtualCamera = tpsCamera->AddComponent<VirtualCamera>(10);
		tpsCamera->AddComponent<LookAtCameraManager>(GetThis<GameObject>());

		springArmComponent->AddHitTag(L"Wall");
		springArmComponent->SetChildObject(tpsCamera);
	}

}