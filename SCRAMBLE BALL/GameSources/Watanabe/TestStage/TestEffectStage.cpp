#include "stdafx.h"
#include "TestEffectStage.h"
#include "../DebugClass/Debug.h"
#include "../Effekseer/EfkEffect.h"
#include "../Camera/TestCamera.h"
#include "../StageObject/CameraTarget.h"
#include "Watanabe/StageObject/StageObjects.h"

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

		GameObjecttCSVBuilder builder;
		builder.Register<Block>(L"Block");
		builder.Register<RackObject>(L"Rack");
		builder.Register<HideItemObject>(L"HideItem");
		builder.Register<GoalObject>(L"Goal");
		builder.Register<OwnAreaObject>(L"OwnAreaObject");
		builder.Register<Container>(L"Container");
		auto dir = App::GetApp()->GetDataDirWString();
		auto path = dir + L"MapDatas/";
		builder.Build(GetThis<Stage>(), path + L"StageS2.csv");

		AddGameObject<SkyBox>(Vec3(500));
	}

	void TestEffectStage::OnUpdate() {
	}
}