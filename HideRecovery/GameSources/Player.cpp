/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"
#include "Player.h"
#include "WaterPressure.h"

#include "LookTargetManager.h"

#include "PlayerMover.h"
#include "PlayerInputer.h"

#include "MaruUtility.h"
#include "GameStageBase.h"
#include "RotationController.h"

#include "PressCenterManager.h"
#include "Pusher.h"

#include "I_Pusher.h"
#include "I_Damaged.h"
#include "PlayerStatusManager.h"

#include "TargetMarkerManager.h"
#include "PlayerController.h"

#include "VelocityManager.h"

#include "DebugObject.h"
#include "ToonPNTStaticModelDraw.h"
#include "PNTToonDraw.h"

#include "WeponBase.h"
#include "WaterWepon.h"

#include "Animator_Player.h"

#include "BoostEffectManager.h"

#include "EffectBase.h"
#include "TackleEffect.h"

#include "Freeze.h"

#include "TimeManager.h"

#include "VernierEffect.h"

#include "GameStageBase.h"

namespace basecross{

	void Player::OnCreate()
	{
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f , 1.0f),
			Vec3(0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -1.0f, 0.0f)
		);

		auto drawComponent = AddComponent<PNTBoneModelDraw>();
		drawComponent->SetMultiMeshResource(L"Player_Idle");
		drawComponent->SetMeshToTransformMatrix(spanMat);

		// プレイヤーの位置情報
		auto transComponent = GetComponent<Transform>();
		transComponent->SetPosition(0.0f, 1.0f, 0.0f); // 初期位置の設定

		// 物理演算の追加
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetAfterCollision(AfterCollision::Auto);
		ptrColl->AddExcludeCollisionTag(L"Water");
		ptrColl->SetMakedSize(1.5f);
		//ptrColl->SetDrawActive(true);

		AddComponent<PlayerAnimator>();

		auto stage = GetGameStage();
		if (!stage)
		{
			return;
		}

		AddComponent<LookTargetManager>();
		m_rotationController = AddComponent<RotationController>();
		//AddComponent<PressCenterManager>();
		AddComponent<Pusher>();
		AddComponent<Gravity>();
		AddComponent<TargetMarkerManager>();
		AddComponent<PlayerStatusManager>();
		AddComponent<PlayerController>();
		AddComponent<VelocityManager>();
		AddComponent<WaterWepon>();
		AddComponent<Effect::Tackle>();
		AddComponent<Freeze>();
		AddComponent<Effect::VernierEffect>();
		AddComponent<TimeManager>();

		//AddComponent<BoostEffectManager>();

		SetAlphaActive(true);
		AddTag(L"Player"); // プレイヤーにタグをつける

		transform->SetScale(Vec3(1.5f));
		//transform->SetScale(Vec3(0.3f));

		//SetDrawLayer(1);

		//影付け
		auto shadow = AddComponent<Shadowmap>();
		//shadow->SetMeshResource(L"Player_Mesh");
		shadow->SetMultiMeshResource(L"Player_Idle");
		shadow->SetMeshToTransformMatrix(spanMat);
	}

	void Player::OnUpdate()
	{

	}


}
//end basecross