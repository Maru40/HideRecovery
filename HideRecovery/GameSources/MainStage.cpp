/*!
@file MainStage.cpp
@brief MainStage実体
*/

#include "stdafx.h"
#include "Project.h"

#include "MainStage.h"
#include "PlayerInputer.h"

#include "EnemyObjectBase.h"
#include "WallObject.h"

#include "DebugObject.h"

#include "OriginalMeshStageObject.h"

#include "ArrowObject.h"

#include "ParticleManager.h"

#include "SoundManager.h"

#include "Mathf.h"

#include "EnemyObjectBase.h"

#include "EffectBase.h"

#include "BoardObject.h"
#include "BillBoard_Ex.h"

#include "AuraObjectBase.h"
#include "AuraBase.h"

#include "EffectManager.h"

#include "EnemyBase.h"

#include "TaskList.h"

#include "EnemyGeneratorObject.h"

#include "DebugNumber.h"
#include "NumbersObject.h"
#include "NumbersCtrl.h"

#include "CameraHelper.h"

#include "HideItemObject.h"

#include "HeroPlayerObject.h"
#include "VillainPlayerObject.h"
#include "Watanabe/DebugClass/Debug.h"
#include "Watanabe/Effekseer/EfkEffect.h"

#include "Itabashi/OnlineTestRoom.h"
#include "Itabashi/OnlinePlayerManager.h"
#include "GameManagerObject.h"
#include "Itabashi/GamePlayerManager.h"
#include "Itabashi/OnlineGameTimer.h"
#include "Watanabe/UI/GameStartUI.h"

using namespace basecross::Enemy;

namespace basecross {
	//wstring MainStage::sm_loadMapName = L"StageS1_Copy.csv";
	wstring MainStage::sm_loadMapName = L"StageS2.csv";
	//wstring MainStage::sm_loadMapName = L"TestOwnArea.csv";

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
			//AddGameObject<Skybox2>();

			//ビューとライトの作成
			CreateViewLight();

			auto cameraObj = Instantiate<CameraObject>();

			EventSystem::GetInstance(GetThis<Stage>())->SetBasicInputer(PlayerInputer::GetInstance());
			AddGameObject<GameManagerObject>();

			auto onlineRoom = AddGameObject<Online::OnlineTestRoom>();
			auto tester = onlineRoom->GetComponent<Online::OnlineTester>();
			auto onlineGameTimer = onlineRoom->AddComponent<OnlineGameTimer>();
			onlineRoom->AddComponent<GamePlayerManager>();

			auto gameStartUI = AddGameObject<GameStartUI>();
			std::weak_ptr<GameStartUI> weakgameStartUI = gameStartUI;
			onlineGameTimer->AddGameStartCountFuncs([weakgameStartUI]() {weakgameStartUI.lock()->Start(); });
			gameStartUI->AddTimeUpEventFunc([]() {SimpleSoundManager::ChangeBGM(L"GameStageBGM", 0.25f); });

			//ステージの設定
			auto scene = App::GetApp()->GetScene<Scene>();
			scene->SetStageName(sm_loadMapName);

			// BGMの再生

			//Generatorの生成
			//AddGameObject<EnemyGeneratorObject>();

			//Mapの読み込み
			CreateMap(sm_loadMapName);
			// UIレイアウトの読み込み
			CreateUI(L"GameUILayout.csv");

			m_gameStartUI = AddGameObject<GameStartUI>();
			m_gameFinishUI = AddGameObject<GameFinishUI>();
			//隠すアイテムの設定
			//Instantiate<HideItemObject>(Vec3(0.0f, 0.0f, 0.0f), Quat::Identity());

			//デバッグ
			//AddGameObject<DebugObject>();

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
		// デバッグ用
		const auto& keyBoard = App::GetApp()->GetMyInputDevice()->GetKeyBoard();
		if (keyBoard.IsInputDown(KeyCode::Alpha1)) {
			m_gameStartUI->Start();
			Debug::GetInstance()->Log(L"StartUI");
		}
		if (keyBoard.IsInputDown(KeyCode::Alpha2)) {
			m_gameFinishUI->Start();
			Debug::GetInstance()->Log(L"FinishUI");
		}
		if (keyBoard.IsInputDown(KeyCode::Alpha0)) {
			m_gameStartUI->Reset();
			m_gameFinishUI->Reset();
			Debug::GetInstance()->Log(L"ResetUI");
		}
	}

	void MainStage::SetLoadMapName(const wstring& name) {
		sm_loadMapName = name;
	}

	wstring MainStage::GetLoadMapName() {
		return sm_loadMapName;
	}

	std::shared_ptr<GameStartUI> MainStage::GetGameStartUI() {
		return m_gameStartUI;
	}
	std::shared_ptr<GameFinishUI> MainStage::GetGameFinishUI() {
		return m_gameFinishUI;
	}
}
//end basecross