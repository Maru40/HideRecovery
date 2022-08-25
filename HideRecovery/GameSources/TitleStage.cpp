#include "stdafx.h"
#include "TitleStage.h"
#include "PlayerInputer.h"
#include "Scene.h"

#include "Watanabe/DebugClass/Debug.h"
#include "Watanabe/UI/UIObjects.h"
#include "Watanabe/StageObject/StageObjects.h"

namespace basecross {
	void TitleStage::CreateViewLight() {
		const Vec3 eye(20.0f, 20.0f, -20.0f);
		const Vec3 at(5.0f);
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

	void TitleStage::OnCreate() {
		CreateViewLight();
		AddGameObject<Debug>();
		Debug::GetInstance()->Log(L"TitleStage");

		GameObjecttCSVBuilder builder;
		builder.Register<Block>(L"Block");
		builder.Register<RackObject>(L"Rack");
		builder.Register<HideItemObject>(L"HideItem");
		builder.Register<PlayerSpawnPointObject>(L"PlayerSpawnPoint");
		builder.Register<GoalObject>(L"Goal");
		builder.Register<OwnAreaObject>(L"OwnAreaObject");
		builder.Register<Container>(L"Container");
		auto dir = App::GetApp()->GetDataDirWString();
		auto path = dir + L"MapDatas/";
		builder.Build(GetThis<Stage>(), path + L"StageS2.csv");

		UIObjectCSVBuilder uiBuilder;
		uiBuilder.Register<SimpleSprite>(L"SimpleSprite");
		uiBuilder.Build(GetThis<Stage>(), path + L"TitleUILayout.csv");

		//auto titleLogo = uiBuilder.GetUIObject<SimpleSprite>(L"TitleLogo");
		//auto drawComp = titleLogo->GetComponent<PCTSpriteDraw>();
		//if (drawComp) {
		//	drawComp->SetDiffuse(Col4(0, 1, 1, 1));
		//}
		//auto pressA = uiBuilder.GetUIObject<SimpleSprite>(L"PressA");
		//drawComp = pressA->GetComponent<PCTSpriteDraw>();
		//if (drawComp) {
		//	drawComp->SetDiffuse(Col4(0, 0, 0, 1));
		//}
	}
	void TitleStage::OnUpdate() {
		const auto& inputDevice = App::GetApp()->GetMyInputDevice();
		const auto& pad = inputDevice->GetXInputGamePad();
		if (pad.IsInputDown(XInputCode::A)) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToMatchingStage");
		}
	}
	void TitleStage::OnDestroy() {
	}
}