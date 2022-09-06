#include "stdafx.h"

#include "MapShotStage.h"
#include "PlayerInputer.h"
#include "Scene.h"

#include "Watanabe/DebugClass/Debug.h"
#include "Watanabe/UI/UIObjects.h"
#include "Watanabe/Component/Zooming.h"
#include "Watanabe/Camera/TitleCamera.h"

#include "PlayerInputer.h"
#include "Maruyama/Utility/Utility.h"

#include "Maruyama/Player/Component/OwnArea.h"

namespace basecross {
	void MapShotStage::CreateViewLight() {
		const Vec3 eye(0.0f, 252.4f, -0.001f);
		const Vec3 at(0, 0.0f, 0);
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

	void MapShotStage::OnCreate() {
		CreateViewLight();
		AddGameObject<Debug>();
		Debug::GetInstance()->Log(L"MapShotStage");

		CreateMap(L"StageS2.csv");

		//auto uiBuilder = CreateUI(L"TitleUILayout.csv");
		//auto pressA = uiBuilder->GetUIObject(L"Press A");
		//pressA->AddComponent<Zooming>();

		//SimpleSoundManager::ChangeBGM(L"TitleStageBGM", 0.1f);

		for (auto& obj : maru::Utility::FindGameObjects<StageObjectBase>(GetThis<Stage>())) {
			auto shadow = obj->GetComponent<Shadowmap>(false);
			if (shadow) {
				shadow->SetDrawActive(false);
				shadow->SetUpdateActive(false);
			}

			if (auto draw = obj->GetComponent<PNTStaticDraw>(false)) {
				draw->SetOwnShadowActive(false);
			}

			if (auto draw = obj->GetComponent<PNTStaticModelDraw>(false)) {
				draw->SetOwnShadowActive(false);
			}

			if (auto ownArea = obj->GetComponent<OwnArea>(false)) {
				auto trans = ownArea->GetGameObject()->GetComponent<Transform>();
				auto pos = trans->GetPosition();
				pos += Vec3::Up() * 1.0f;
				trans->SetPosition(pos);
			}
		}
	}
	void MapShotStage::OnUpdate() {
		// マッチング画面への遷移処理
		const auto& inputDevice = App::GetApp()->GetMyInputDevice();
		const auto& pad = inputDevice->GetXInputGamePad();

		auto camera = GetView()->GetTargetCamera();

		if (pad.IsInputDown(XInputCode::A)) {
			Debug::GetInstance()->Log(camera->GetEye());
		}

		auto input = PlayerInputer::GetInstance()->GetMoveDirection();
		if (input != Vec2(0)) {
			auto pos = camera->GetEye();
			pos.y += input.y;
			camera->SetEye(pos);
		}

		//if (pad.IsInputDown(XInputCode::A)) {
		//	SimpleSoundManager::OnePlaySE(L"TitlePlessedSE", 0.25f);
		//	PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToMatchingStage");
		//}
	}
	void MapShotStage::OnDestroy() {
	}
}