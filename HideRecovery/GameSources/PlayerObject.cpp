
/*!
@file PlayerObject.cpp
@brief PlayerObject�N���X����
�S���F�ێR�T��
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
#include "OwnHideItemManager.h"

namespace basecross {

	PlayerObject::PlayerObject(const std::shared_ptr<Stage>& stage):
		GameObject(stage)
	{}

	void PlayerObject::OnCreate() {
		Mat4x4 spanMat;
		Vec3 scale = Vec3(0.5f);
		spanMat.affineTransformation(
			scale,
			Vec3(0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		auto draw = AddComponent<PNTBoneModelDraw>();
		draw->SetMultiMeshResource(L"Player_Mesh");
		draw->SetMeshToTransformMatrix(spanMat);
		SetAlphaActive(true);

		transform->SetScale(Vec3(1.0f, 1.5f, 1.0f));

		AddComponent<CollisionObb>();
		auto playerMover = AddComponent<PlayerMover>();
		playerMover->SetIsCameraAffected(true);
		AddComponent<RotationController>();
		AddComponent<PlayerController>();
		AddComponent<Gravity>();
		
		AddComponent<ItemBag>();
		AddComponent<ItemAcquisitionManager>();
		AddComponent<OwnHideItemManager>();

		//�J�����Z�b�e�B���O----------------------------------------------------------

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
		tpsCamera->AddComponent<LookAtCameraManager>(GetThis<GameObject>(), LookAtCameraManager::Parametor());

		springArmComponent->AddHitTag(L"Wall");
		springArmComponent->SetChildObject(tpsCamera);
	}

}