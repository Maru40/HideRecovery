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
#include "../Component/TestComponent.h"
#include "../Camera/TitleCamera.h"

#include "../Shader/BoneModelDraw.h"
#include "../Shader/StaticModelDraw.h"

namespace basecross {
	void TestShaderStage::CreateViewLight() {
		const Vec3 eye(5.0f, 5.0f, -5.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<TitleCamera>(eye, at);
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetSpeed(-0.5f);
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
		CreatePlayer();
		// 静的オブジェクト
		CreateSphere();
		// 念のためSkyBoxを生成
		AddGameObject<SkyBox>(Vec3(100, 100, 100));
	}

	void TestShaderStage::OnUpdate() {
	}

	shared_ptr<GameObject> TestShaderStage::CreatePlayer() {
		auto player = AddGameObject<GameObject>();
		auto drawComp = player->AddComponent<BoneModelDraw>();
		drawComp->SetMultiMeshResource(L"Player_Mesh");

		auto shadow = player->AddComponent<Shadowmap>();
		shadow->SetMultiMeshResource(L"Player_Mesh");

		// テスト用の回転処理を実装
		auto testComp = player->AddComponent<TestComponent>();
		testComp->SetOnUpdateFunction(
			[player](float delta) {
				auto util = player->GetBehavior<UtilBehavior>();
				util->RotToHead(Vec3(cosf(delta), 0, sinf(delta)), 2 * delta);
			}
		);

		auto transComp = player->GetComponent<Transform>();
		transComp->SetPosition(Vec3(-1, 0, 0));

		return player;
	}

	shared_ptr<GameObject> TestShaderStage::CreateSphere() {
		auto sphere = AddGameObject<GameObject>();
		auto drawComp = sphere->AddComponent<StaticModelDraw>();
		drawComp->SetMeshResource(L"DEFAULT_SPHERE");
		// 今現在テクスチャが無いとトゥーンシェーダーが実行されない
		drawComp->SetTextureResource(L"Floor_TX");

		auto tex = drawComp->GetAllTextureResource();

		auto shadow = sphere->AddComponent<Shadowmap>();
		shadow->SetMeshResource(L"DEFAULT_SPHERE");

		auto transComp = sphere->GetComponent<Transform>();
		transComp->SetPosition(Vec3(1, 0.5, 0));

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