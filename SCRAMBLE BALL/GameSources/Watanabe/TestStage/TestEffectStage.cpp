#include "stdafx.h"
#include "TestEffectStage.h"
#include "../DebugClass/Debug.h"
#include "../Effekseer/EfkEffect.h"

namespace basecross {
	void TestEffectStage::CreateViewLight() {
		const Vec3 eye(5.0f, 5.0f, -5.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
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
	}

	void TestEffectStage::OnUpdate() {
	}
}