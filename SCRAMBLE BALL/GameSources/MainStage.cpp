/*!
@file MainStage.cpp
@brief MainStage実体
*/

#include "stdafx.h"
#include "Project.h"

#include "MainStage.h"
#include "Patch/PlayerInputer.h"

#include "Maruyama/Utility/SingletonComponent/SoundManager.h"

#include "Maruyama/Utility/Mathf.h"

#include "Patch/CameraHelper.h"

#include "Maruyama/Item/HideItemObject.h"

#include "Maruyama/Player/Object/VillainPlayerObject.h"
#include "Watanabe/DebugClass/Debug.h"
#include "Watanabe/Effekseer/EfkEffect.h"

#include "Maruyama/Utility/SingletonComponent/GameManager.h"
#include "Itabashi/GamePlayerManager.h"
#include "Itabashi/MainStageTransitioner.h"
#include "Itabashi/OnlineGameTimer.h"
#include "Itabashi/OnlineTestRoom.h"
#include "Watanabe/Manager/TimeManager.h"
#include "Watanabe/UI/GameStartUI.h"

#include "Watanabe/UI/SplashMessageUI.h"
#include "Watanabe/UI/UIObjectCSVBuilder.h"

#include "Maruyama/StageObject/HidePlace.h"
#include "Maruyama/UI/2D/Component/Reticle.h"
#include "Maruyama/UI/2D/Component/TeleportUI.h"

#include "Maruyama/Utility/SingletonComponent/GameManager.h"
#include "Maruyama/Utility/Object/GameManagerObject.h"
#include "Maruyama/Interface/I_TeamMember.h"

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
		HidePlace::CountReset();

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

			auto onlineRoom = AddGameObject<Online::OnlineTestRoom>();
			auto tester = onlineRoom->GetComponent<Online::OnlineTester>();
			auto onlineGameTimer = onlineRoom->AddComponent<OnlineGameTimer>();
			onlineRoom->AddComponent<GamePlayerManager>();
			onlineRoom->AddComponent<Online::MainStageTransitioner>();

			auto gameStartUI = AddGameObject<GameStartUI>();
			std::weak_ptr<GameStartUI> weakgameStartUI = gameStartUI;
			onlineGameTimer->AddGameStartCountFunc([weakgameStartUI]() { weakgameStartUI.lock()->Start(); });
			gameStartUI->AddTimeUpEventFunc([]() { SimpleSoundManager::ChangeBGM(L"GameStageBGM", 0.05f); });
			gameStartUI->AddTimeUpEventFunc([]() { SimpleSoundManager::OnePlaySE(L"GameStartSE", 0.25f); });
			std::weak_ptr<OnlineGameTimer> weakOnlineGameTimer = onlineGameTimer;
			gameStartUI->AddTimeUpEventFunc([weakOnlineGameTimer]() { weakOnlineGameTimer.lock()->GameTimerStart(); });
			//ゲーム状態にする
			gameStartUI->AddTimeUpEventFunc([]() { if (auto gameManager = GameManager::GetInstance()) { gameManager->ChangeState(GameManager::State::Game); } });

			auto gameFinishUI = AddGameObject<GameFinishUI>();
			std::weak_ptr<GameFinishUI> weakGameFinishUI = gameFinishUI;
			onlineGameTimer->AddGameFinishCountEventFunc([weakGameFinishUI]() { weakGameFinishUI.lock()->Start(); });
			onlineGameTimer->AddGameFinishEventFunc([]() { SimpleSoundManager::OnePlaySE(L"GameSetSE", 0.25f); });

			//ステージの設定
			auto scene = App::GetApp()->GetScene<Scene>();
			scene->SetStageName(sm_loadMapName);

			// BGMの再生

			// Generatorの生成
			// AddGameObject<EnemyGeneratorObject>();

			// Mapの読み込み
			CreateMap(sm_loadMapName);

			// UIレイアウトの読み込み
			auto gameUIBuilder = CreateUI(L"GameUILayout.csv");
			auto remainingTime = gameUIBuilder->GetUIObject<SplashMessageUI>(L"RemainingTime");
			auto teamRed = gameUIBuilder->GetUIObject<SimpleSprite>(L"TeamRed");
			teamRed->GetDrawComponent()->SetDiffuse(team::REDTEAM_COLOR);
			auto teamBlue = gameUIBuilder->GetUIObject<SimpleSprite>(L"TeamBlue");
			teamBlue->GetDrawComponent()->SetDiffuse(team::BLUETEAM_COLOR);

			// 残り時間表示のイベント登録
			TimeManager::GetInstance()->AddEvent(60,
				[remainingTime]() {
					remainingTime->SetColor(Col4(1, 1, 1, 0.5f));
					remainingTime->SetMessage(SplashMessageUI::MessageType::Remaining60s);
				});
			TimeManager::GetInstance()->AddEvent(30,
				[remainingTime]() {
					remainingTime->SetColor(Col4(1, 1, 1, 0.5f));
					remainingTime->SetMessage(SplashMessageUI::MessageType::Remaining30s);
				});

			Debug::GetInstance()->Log(L"入力------------------------------");
			Debug::GetInstance()->Log(L"X  : 置く");
			Debug::GetInstance()->Log(L"A  : 拾う");
			Debug::GetInstance()->Log(L"B  : ブラフ");
			Debug::GetInstance()->Log(L"↓ : 置く場所制限切り替え");
			Debug::GetInstance()->Log(L"----------------------------------");
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

	// std::shared_ptr<GameStartUI> MainStage::GetGameStartUI() {
	//	return m_gameStartUI;
	// }
	// std::shared_ptr<GameFinishUI> MainStage::GetGameFinishUI() {
	//	return m_gameFinishUI;
	// }
}
// end basecross