/*!
@file MainStage.cpp
@brief MainStage実体
*/

#include "stdafx.h"
#include "Project.h"

#include "MainStage.h"
#include "Patch/PlayerInputer.h"

#include "Patch/CameraHelper.h"

#include "Watanabe/DebugClass/Debug.h"
#include "Watanabe/Effekseer/EfkEffect.h"

#include "Maruyama/Utility/SingletonComponent/GameManager.h"
#include "Itabashi/OnlineGameTimer.h"
#include "Watanabe/Manager/TimeManager.h"
#include "Watanabe/UI/GameStartUI.h"

#include "Watanabe/UI/SplashMessageUI.h"
#include "Watanabe/UI/UIObjectCSVBuilder.h"

#include "Maruyama/Utility/SingletonComponent/GameManager.h"
#include "Maruyama/Utility/Object/GameManagerObject.h"
#include "Maruyama/Interface/I_TeamMember.h"

#include "Itabashi/MainStageCoreObject.h"
#include "Itabashi/MainStageUIObject.h"
#include "Itabashi/MainStageTransitioner.h"

#include "Itabashi/DisconnectToTitleUIObject.h"
#include "Itabashi/MainStageDisconnectUIController.h"
#include "Itabashi/OnlineAliveChecker.h"

#include "Maruyama/Enemy/Behavior/Tester/Object/BehaviorTesterObject.h"

namespace basecross {
	// wstring MainStage::sm_loadMapName = L"StageS1_Copy.csv";
	wstring MainStage::sm_loadMapName = L"StageS2.csv";
	// wstring MainStage::sm_loadMapName = L"TestOwnArea.csv";

	void MainStage::CreateViewLight() {
		CreateStartCamera(sm_loadMapName);
		CreateMainCamera();
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();

		ChangeMainCamera(); //初期カメラの設定
	}

	void MainStage::OnCreate() {
		try {
			AddGameObject<EfkInterface>();
			AddGameObject<Debug>();
			Debug::GetInstance()->Log(L"MainStage");
			// スカイボックス
			// AddGameObject<Skybox2>();

			//ビューとライトの作成
			CreateViewLight();

			auto cameraObj = Instantiate<CameraObject>();

			EventSystem::GetInstance(GetThis<Stage>())->SetBasicInputer(PlayerInputer::GetInstance());
			AddGameObject<GameManagerObject>();

			auto coreObject = AddGameObject<StageObject::MainStageCoreObject>();
			auto onlineGameTimer = coreObject->GetComponent<OnlineGameTimer>();
			auto mainStageTransitioner = coreObject->GetComponent<Online::MainStageTransitioner>();
			auto onlineAliveChecker = coreObject->GetComponent<Online::OnlineAliveChecker>();

			auto uiObject = AddGameObject<StageObject::MainStageUIObject>();

			auto gameStartUI = uiObject->GetGameStartUI();
			std::weak_ptr<GameStartUI> weakgameStartUI = gameStartUI;
			onlineGameTimer->AddGameStartCountFunc([weakgameStartUI]() { weakgameStartUI.lock()->Start(); });

			std::weak_ptr<OnlineGameTimer> weakOnlineGameTimer = onlineGameTimer;
			gameStartUI->AddTimeUpEventFunc([weakOnlineGameTimer]() { weakOnlineGameTimer.lock()->GameTimerStart(); });

			auto gameFinishUI = uiObject->GetGameFinishUI();
			std::weak_ptr<GameFinishUI> weakGameFinishUI = gameFinishUI;
			onlineGameTimer->AddGameFinishCountEventFunc([weakGameFinishUI]() { weakGameFinishUI.lock()->Start(); });

			mainStageTransitioner->SetDisconnectToTitleUIObject(uiObject->GetDisconnectToTitleUIObject());

			uiObject->GetDisconnectToTitleUIObject()->GetComponent<MainStageDisconnectUIController>()->SetOnlineAliveChecker(onlineAliveChecker);

			//ステージの設定
			auto scene = App::GetApp()->GetScene<Scene>();
			scene->SetStageName(sm_loadMapName);

			// BGMの再生

			// Generatorの生成
			// AddGameObject<EnemyGeneratorObject>();

			// UIレイアウトの読み込み
			auto gameUIBuilder = CreateUI(L"GameUILayout.csv");
			std::weak_ptr<SplashMessageUI> weakRemainingTime = gameUIBuilder->GetUIObject<SplashMessageUI>(L"RemainingTime");
			auto teamRed = gameUIBuilder->GetUIObject<SimpleSprite>(L"TeamRed");
			teamRed->GetDrawComponent()->SetDiffuse(team::REDTEAM_COLOR);
			auto teamBlue = gameUIBuilder->GetUIObject<SimpleSprite>(L"TeamBlue");
			teamBlue->GetDrawComponent()->SetDiffuse(team::BLUETEAM_COLOR);

			// 残り時間表示のイベント登録
			TimeManager::GetInstance()->AddEvent(60,
				[weakRemainingTime]() {
					auto remainingTime = weakRemainingTime.lock();
					remainingTime->SetColor(Col4(1, 1, 1, 0.5f));
					remainingTime->SetMessage(SplashMessageUI::MessageType::Remaining60s);
				}
			);
			TimeManager::GetInstance()->AddEvent(30,
				[weakRemainingTime]() {
					auto remainingTime = weakRemainingTime.lock();
					remainingTime->SetColor(Col4(1, 1, 1, 0.5f));
					remainingTime->SetMessage(SplashMessageUI::MessageType::Remaining30s);
				}
			);

			// Mapの読み込み
			CreateMap(sm_loadMapName);

			//AddGameObject<maru::Behavior::BehaviorTesterObject>();

			//Debug::GetInstance()->Log(L"入力------------------------------");
			//Debug::GetInstance()->Log(L"X  : 置く");
			//Debug::GetInstance()->Log(L"A  : 拾う");
			//Debug::GetInstance()->Log(L"B  : ブラフ");
			//Debug::GetInstance()->Log(L"↓ : 置く場所制限切り替え");
			//Debug::GetInstance()->Log(L"----------------------------------");
		}
		catch (...) {
			throw;
		}
	}

	void MainStage::OnUpdate() {
	}

	void MainStage::SetLoadMapName(const wstring& name) {
		sm_loadMapName = name;
	}

	wstring MainStage::GetLoadMapName() {
		return sm_loadMapName;
	}
}
// end basecross