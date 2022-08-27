#include "stdafx.h"
#include "MatchingUILayoutStage.h"
#include "../UI/UIObjects.h"
#include "../DebugClass/Debug.h"
#include "InputHelper.h"
#include "Scene.h"
#include "PlayerInputer.h"
#include "../UI/UIObjects.h"
#include "../Manager/PointManager.h"
#include "../Manager/ScoreManager.h"
#include "../BoardPoly/HPGaugeBP.h"

namespace basecross {
	void MatchingUILayoutStage::CreateViewLight() {
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
	void MatchingUILayoutStage::OnCreate() {
		CreateViewLight();
		AddGameObject<Debug>();
		Debug::GetInstance()->Log(L"MatchingUILayoutStage");

		UIObjectCSVBuilder uiBuilder;
		uiBuilder.Register<TimerUI>(L"TimerUI");
		uiBuilder.Register<HPGaugeUI>(L"HPGaugeUI");
		uiBuilder.Register<PointUI>(L"PointUI");
		uiBuilder.Register<SimpleSprite>(L"SimpleSprite");
		uiBuilder.Register<WinOrLoseUI>(L"WinOrLoseUI");
		uiBuilder.Register<SplashMessageUI>(L"SplashMessageUI");
		uiBuilder.Register<Numbers>(L"Numbers");
		auto dir = App::GetApp()->GetDataDirWString();
		auto path = dir + L"MapDatas/";
		uiBuilder.Build(GetThis<Stage>(), path + L"TestUILayout.csv");

		ScoreManager::GetInstance()->AddKillCount(0);
		//AddGameObject<ScoreUI>(0);
	}
	void MatchingUILayoutStage::OnUpdate() {
		const auto& inputDevice = App::GetApp()->GetMyInputDevice();
		const auto& pad = inputDevice->GetXInputGamePad();
		if (pad.IsInputDown(XInputCode::Start)) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToWatanabeStage");
		}
		if (pad.IsInputDown(XInputCode::A)) {
			//m_sprite->SetMessage(SplashMessageUI::MessageType::GetBall);
		}
	}
}