#include "stdafx.h"
#include "Project.h"
#include "ResultStage.h"
#include "Result.h"
#include "StageComplete.h"
#include "Score.h"
#include "ResultTime.h"
#include "ResultBoard.h"
#include "SoundManager.h"
#include "Player.h"
#include "GameManagerObject.h"
#include "MainStage.h"

#include "Animator_Player.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	リザルトステージクラス実体
	//--------------------------------------------------------------------------------------
	void ResultStage::CreateViewLight() {
		const Vec3 eye(0.0f, 0.0f, -20.0f);
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



	void ResultStage::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();

			AddGameObject<GameManagerObject>();

			//AddGameObject<GameObject>()->AddComponent<SoundManager>();
			//SoundManager::GetInstance(GetThis<Stage>())->PlayOneShot();

			//SEStop
			//SoundManager::GetInstance()->Stop(SoundManager::ID::GameBGM);
			//SoundManager::GetInstance()->Stop(SoundManager::ID::ClearSE);
			auto& app = App::GetApp();
			auto path = app->GetDataDirWString();
			// テクスチャの読み込み
			auto texPath = path + L"Textures/";
			// テクスチャの登録
			auto tpath = app->GetDataDirWString() + L"Textures/";
			app->RegisterTexture(L"ResultBackGround", tpath + L"ResultBackGround.png"); // 背景

			app->RegisterTexture(L"Result", tpath + L"Result.png"); // Gaugeテクスチャ
			app->RegisterTexture(L"ResultStageClear", tpath + L"ResultStageClear.png"); // StageCompleteテクスチャ

						// SEの再生
			//AddGameObject<GameObject>()->AddComponent<SoundManager>();
			SoundManager::GetInstance(GetThis<Stage>())->PlayBGM(SoundManager::ID::ClearSE, 0.15f, 0);
		}
		catch (...) {
			throw;
		}

		// フェードイン及びフェードアウトの追加
		m_FU = AddGameObject<FadeUI>();
		// Result背景の追加
		AddGameObject<ResultBoard>();

		// ゲームオブジェクトの追加
		auto result = AddGameObject<Result>();
		auto stageComplete = AddGameObject<StageComplete>();
		auto time = AddGameObject<ResultTime>();
		auto timeM = AddGameObject<ResultTimeMinute>();
		auto score = AddGameObject<Score>();
		auto player = AddGameObject<Player>();
		auto playertrans = player->GetComponent<Transform>();
		auto resultrank = AddGameObject<ResultRank>();
		auto resultNext = AddGameObject<ResultNext>();
		auto resultRetry = AddGameObject<ResultRetry>();
		auto abotton = AddGameObject<ABotton>();
		auto bbotton = AddGameObject<BBotton>();
		playertrans->SetPosition(+30.0f, -3.0f, 0.0f);
		//playertrans->SetRotation(-0.4f, 3.0f, 0.1f);
		playertrans->SetForward(Vec3(0.0f, 0.0f, -1.0f));
		playertrans->SetScale(5.0f, 5.0f, 5.0f);

		auto animator = player->GetComponent<PlayerAnimator>(false);
		if (animator) {
			animator->ChangeState(PlayerAnimator::StateEnum::WinMove);
		}
	}

	void ResultStage::OnUpdate()
	{
		// アプリの参照取得
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		static const auto& inputDevice = app->GetMyInputDevice();
		static const auto& keyBoard = inputDevice->GetKeyBoard();

		// デバイスの取得
		auto device = app->GetInputDevice();
		const auto& pad = device.GetControlerVec()[0];

		// スティックの傾きをベクトルに変更
		Vec3 LeftStick(pad.fThumbLX, 0.0f, pad.fThumbLY);

		// 単位時間の取得
		float deltaTime = app->GetElapsedTime();


		if (!m_sceneflag && pad.wPressedButtons & XINPUT_GAMEPAD_A && m_FU->GetColw() <= 0.0f)
		{
			m_sceneflag = true;
			SoundManager::GetInstance(GetThis<Stage>())->PlayOneShot(SoundManager::ID::EnterSE, 0.3f);
			m_sceneMode = 1;
			m_FU->FadeIn();
		}
		if (m_FU->GetColw() >= 1.0f && m_sceneMode == 1)
		{
			auto soundManager = SoundManager::GetInstance();
			if (soundManager) {
				soundManager->Stop(SoundManager::ID::ClearSE);
			}

			int a = scene->GetStageName();
			MainStage::SetLoadMapName(m_StageName[scene->GetStageName() + 1]); // Stage番号を呼んできて次のステージを読めるようにする
			PostEvent(0.0f, GetThis<ObjectInterface>(),
				App::GetApp()->GetScene<Scene>(), L"ToMainStage");
		}


		if (!m_sceneflag && pad.wPressedButtons & XINPUT_GAMEPAD_B && m_FU->GetColw() <= 0.0f)
		{
			SoundManager::GetInstance(GetThis<Stage>())->PlayOneShot(SoundManager::ID::EnterSE, 0.3f);
			m_sceneflag = true;
			m_sceneMode = 2;
			m_FU->FadeIn();
		}
		if (m_FU->GetColw() >= 1.0f && m_sceneMode == 2)
		{
			auto soundManager = SoundManager::GetInstance();
			if (soundManager) {
				soundManager->Stop(SoundManager::ID::ClearSE);
			}

			int a = scene->GetStageName();
			MainStage::SetLoadMapName(m_StageName[scene->GetStageName()]);
			PostEvent(0.0f, GetThis<ObjectInterface>(),
				App::GetApp()->GetScene<Scene>(), L"ToMainStage");
		}
	}
}
//end basecross