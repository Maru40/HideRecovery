
/*!
@file TestPlayerObject.cpp
@brief TestPlayerObjectクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "PlayerObject.h"
#include "TestPlayerObject.h"

#include "Maruyama/Player/Component/OwnHideItemManager.h"

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
#include "Maruyama/Player/Component/PlayerController.h"

namespace basecross {

	TestPlayerObject::TestPlayerObject(const std::shared_ptr<Stage>& stage) :
		GameObject(stage)
	{}

	void TestPlayerObject::OnCreate() {
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
		//draw->SetMultiMeshResource(L"Player_Mesh");
		draw->SetMeshResource(L"DEFAULT_CUBE");
		draw->SetMeshToTransformMatrix(spanMat);
		draw->SetModelTextureEnabled(false);
		//SetAlphaActive(true);
		//draw->SetDiffuse(Col4(0.0f, 0.0f, 1.0f, 1.0f));

		auto collision = AddComponent<CollisionObb>();
		//collision->SetDrawActive(true);
		constexpr float CollisionSize = 1.0f;
		collision->SetMakedSize(CollisionSize);
		AddComponent<maru::CollisionAction>();
		AddComponent<RotationController>();
		AddComponent<PlayerController>();
		auto gravity = AddComponent<Gravity>();
		gravity->SetGravity(Vec3(0.0f, -9.8f * 3.0f, 0.0f));

		AddComponent<VelocityManager>();
		AddComponent<PlayerMover>();

		AddComponent<PlayerStatus>();
		AddComponent<Teleport>();

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
	}


}