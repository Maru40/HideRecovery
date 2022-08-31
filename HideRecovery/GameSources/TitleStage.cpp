#include "stdafx.h"
#include "TitleStage.h"
#include "PlayerInputer.h"
#include "Scene.h"

#include "Watanabe/DebugClass/Debug.h"
#include "Watanabe/UI/UIObjects.h"
#include "Watanabe/Component/Zooming.h"
#include "Watanabe/Camera/TitleCamera.h"

namespace basecross {
	void TitleStage::CreateViewLight() {
		const Vec3 eye(20.0f, 20.0f, -20.0f);
		const Vec3 at(0, 5.0f, 0);
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<TitleCamera>(eye, at);
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

		CreateMap(L"StageS2.csv");

		auto uiBuilder = CreateUI(L"TitleUILayout.csv");
		auto pressA = uiBuilder->GetUIObject(L"Press A");
		pressA->AddComponent<Zooming>();

		//SimpleSoundManager::ChangeBGM(L"TitleStageBGM", 0.1f);
	}
	void TitleStage::OnUpdate() {
		// マッチング画面への遷移処理
		const auto& inputDevice = App::GetApp()->GetMyInputDevice();
		const auto& pad = inputDevice->GetXInputGamePad();
		if (pad.IsInputDown(XInputCode::A)) {
			SimpleSoundManager::OnePlaySE(L"TitlePlessedSE", 0.25f);
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToMatchingStage");
		}
	}
	void TitleStage::OnDestroy() {
	}
}