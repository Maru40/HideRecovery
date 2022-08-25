#include "stdafx.h"
#include "ResultStage.h"
#include "PlayerInputer.h"
#include "Scene.h"

#include "Watanabe/DebugClass/Debug.h"
#include "Watanabe/UI/UIObjects.h"

#include "../Manager/PointManager.h"
#include "../Manager/TimeManager.h"

namespace basecross {
	void ResultStage::CreateViewLight() {
		const Vec3 eye(20.0f, 5.0f, 0.0f);
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

	void ResultStage::OnCreate() {
		CreateViewLight();
		AddGameObject<Debug>();
		Debug::GetInstance()->Log(L"ResultStage");
		Debug::GetInstance()->Log(L"A : マッチング画面へ");
		Debug::GetInstance()->Log(L"B : タイトル画面へ");

		CreateMap(L"StageS2.csv");

		UIObjectCSVBuilder uiBuilder;
		uiBuilder.Register<TimerUI>(L"TimerUI");
		uiBuilder.Register<HPGaugeUI>(L"HPGaugeUI");
		uiBuilder.Register<PointUI>(L"PointUI");
		uiBuilder.Register<SimpleSprite>(L"SimpleSprite");
		uiBuilder.Register<WinOrLoseUI>(L"WinOrLoseUI");
		auto dir = App::GetApp()->GetDataDirWString();
		auto path = dir + L"MapDatas/";
		uiBuilder.Build(GetThis<Stage>(), path + L"ResultUILayout.csv");

		// 勝敗表示のUIオブジェクトを取得し、チームデータをセット
		//（チームをセットしたあとに表示される）
		auto winOrLose = uiBuilder.GetUIObject<WinOrLoseUI>(L"WinOrLose");
		winOrLose->SetTeam(Team::East); // 仮でデータセット

		// 1ゲーム終了したのでインスタンスを破棄（リセット）
		PointManager::DeleteInstance();
		TimeManager::DeleteInstance();
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
	}
}