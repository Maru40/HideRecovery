#include "stdafx.h"
#include "ResultStage.h"
#include "PlayerInputer.h"
#include "Scene.h"

#include "Watanabe/DebugClass/Debug.h"
#include "Watanabe/UI/UIObjects.h"
#include "Watanabe/Effekseer/EfkEffect.h"

#include "../Manager/PointManager.h"
#include "../Manager/TimeManager.h"
#include "../Manager/ScoreManager.h"
#include "Itabashi/OnlineManager.h"

namespace basecross {
	void ResultStage::CreateViewLight() {
		const Vec3 eye(0.0f, 1.0f, 5.0f);
		const Vec3 at(0, 1.0f, 0);
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

	void ResultStage::OnCreate() {
		CreateViewLight();
		AddGameObject<EfkInterface>();
		AddGameObject<Debug>();
		Debug::GetInstance()->Log(L"ResultStage");
		Debug::GetInstance()->Log(L"A : マッチング画面へ");
		Debug::GetInstance()->Log(L"B : タイトル画面へ");
		PointManager::GetInstance()->AddPoint(team::TeamType::Blue);
		PointManager::GetInstance()->AddPoint(team::TeamType::Red);

		CreateMap(L"WaitStage.csv");
		auto uiBuilder = CreateUI(L"ResultUILayout.csv");
		// ラベルの色変更
		auto redLabel = uiBuilder->GetUIObject<SimpleSprite>(L"RedLabel");
		redLabel->GetDrawComponent()->SetDiffuse(team::REDTEAM_COLOR);
		auto blueLabel = uiBuilder->GetUIObject<SimpleSprite>(L"BlueLabel");
		blueLabel->GetDrawComponent()->SetDiffuse(team::BLUETEAM_COLOR);

		// 紙吹雪エフェクト
		auto effectObject = AddGameObject<GameObject>();
		effectObject->GetComponent<Transform>()->SetPosition(Vec3(0, 5, 0));
		auto efkComp = effectObject->AddComponent<EfkComponent>();
		efkComp->SetEffectResource(L"Confetti");
		efkComp->PlayLoop(L"Confetti");

		Online::OnlineManager::Disconnect();
	}

	void ResultStage::OnUpdate() {
		// 各画面への遷移処理
		// Aでマッチング画面、Bでタイトル画面
		const auto& inputDevice = App::GetApp()->GetMyInputDevice();
		const auto& pad = inputDevice->GetXInputGamePad();
		if (pad.IsInputDown(XInputCode::A)) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToMatchingStage");
		}
		else if (pad.IsInputDown(XInputCode::B)) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
		}
	}

	void ResultStage::OnDestroy() {
		// リザルトを抜ける際にインスタンスを破棄（リセット）
		PointManager::DeleteInstance();
		TimeManager::DeleteInstance();
		ScoreManager::DeleteInstance();
	}
}