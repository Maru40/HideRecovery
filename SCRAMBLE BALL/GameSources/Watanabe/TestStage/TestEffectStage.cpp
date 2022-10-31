#include "stdafx.h"
#include "TestEffectStage.h"
#include "../DebugClass/Debug.h"
#include "../Effekseer/EfkEffect.h"
#include "../Camera/TestCamera.h"
#include "../StageObject/CameraTarget.h"

namespace basecross {
	void TestEffectStage::CreateViewLight() {
		const Vec3 eye(5.0f, 5.0f, -5.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<GameCamera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetTarget(AddGameObject<CameraTarget>(PtrCamera));
		//PtrCamera->SetNear(5);
		//PtrCamera->SetFar(10);
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}

	void TestEffectStage::OnCreate() {
		CreateViewLight();
		AddGameObject<Debug>();
		Debug::GetInstance()->Log(L"TestEffectStage");
		AddGameObject<EfkInterface>();

		auto test = AddGameObject<GameObject>();
		auto draw = test->AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");
	}

	void TestEffectStage::OnUpdate() {
	}
}