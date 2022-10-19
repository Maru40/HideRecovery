/*!
@file   TestShaderStage.cpp
@brief  シェーダー用のテストステージ実体
*/

#include "stdafx.h"
#include "TestShaderStage.h"

#include "../Component/PlayerAnimator.h"
#include "../DebugClass/Debug.h"
#include "VelocityManager.h"
#include "../StageObject/SkyBox.h"

namespace basecross {
	void TestShaderStage::CreateViewLight() {
		const Vec3 eye(5.0f, 5.0f, -5.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}

	void TestShaderStage::OnCreate() {
		CreateViewLight();
		AddGameObject<Debug>();
		Debug::GetInstance()->Log(L"TestShaderStage");

		// 床を生成
		CreateFloor();
		// プレイヤーを生成
		//CreatePlayer();
		// 静的オブジェクト
		CreateSphere();
		// 念のためSkyBoxを生成
		AddGameObject<SkyBox>(Vec3(100, 100, 100));
	}

	void TestShaderStage::OnUpdate() {
	}

	shared_ptr<GameObject> TestShaderStage::CreatePlayer() {
		auto player = AddGameObject<GameObject>();
		auto drawComp = player->AddComponent<PNTBoneModelDraw>();
		drawComp->SetMultiMeshResource(L"Player_Mesh");

		auto shadow = player->AddComponent<Shadowmap>();
		shadow->SetMultiMeshResource(L"Player_Mesh");

		auto animator = player->AddComponent<PlayerAnimator>();
		animator->ChangePlayerAnimation(
			PlayerAnimationState::State::Wait
		);

		// PlayerAnimatorが参照している
		player->AddComponent<VelocityManager>();

		return player;
	}

	shared_ptr<GameObject> TestShaderStage::CreateSphere() {
		auto sphere = AddGameObject<GameObject>();
		auto drawComp = sphere->AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_SPHERE");

		auto shadow = sphere->AddComponent<Shadowmap>();
		shadow->SetMeshResource(L"DEFAULT_SPHERE");

		auto transComp = sphere->GetComponent<Transform>();
		transComp->SetPosition(Vec3(0, 0.5, 0));

		return sphere;
	}

	shared_ptr<GameObject> TestShaderStage::CreateFloor() {
		auto box = AddGameObject<GameObject>();
		auto drawComp = box->AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		drawComp->SetOwnShadowActive(true);

		auto transComp = box->GetComponent<Transform>();
		transComp->SetScale(Vec3(10, 1, 10));
		transComp->SetPosition(Vec3(0, -0.5, 0));

		return box;
	}
}