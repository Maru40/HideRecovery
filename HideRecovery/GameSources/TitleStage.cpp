#include "stdafx.h"
#include "Project.h"
#include "TitleStage.h"
#include "TitleBoard.h"

#include "SingletonComponent.h"
#include "SoundManager.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	リザルトステージクラス実体
	//--------------------------------------------------------------------------------------
	void TitleStage::CreateViewLight() {
		const Vec3 eye(0.0f, 15.0f, -20.0f);
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

	void TitleStage::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();

			auto& app = App::GetApp();
			auto path = app->GetDataDirWString();
			// テクスチャの読み込み
			auto texPath = path + L"Textures/";
			// テクスチャの登録
			auto tpath = app->GetDataDirWString() + L"Textures/";
			app->RegisterTexture(L"Result", tpath + L"Result.png"); // Gaugeテクスチャ
			app->RegisterTexture(L"ResultStageClear", tpath + L"ResultStageClear.png"); // StageCompleteテクスチャ
			app->RegisterTexture(L"TitleBackGround", tpath + L"TitleBackGround.png"); // 背景
			app->RegisterTexture(L"GameTitle", tpath + L"GameTitle.png"); // タイトル
			app->RegisterTexture(L"GameStart", tpath + L"GameStart.png"); // タイトル
			app->RegisterTexture(L"Exit", tpath + L"Exit.png"); // タイトル
		}
		catch (...) {
			throw;
		}
		//SoundManagerの実装
		AddGameObject<GameObject>()->AddComponent<SoundManager>();
		const float Volume = 0.15f;
		SoundManager::GetInstance(GetThis<Stage>())->PlayBGM(SoundManager::ID::TitleBGM, Volume);

		// Title背景の追加
		AddGameObject<TitleBoard>();
		// ゲームオブジェクトの追加

		m_CursorUi = AddGameObject<Cursor>();

		// Uiの追加
		std::vector<CursorUIData> titleDatas = {
			CursorUIData(UiData(L"PauseBack", Vec3(0.0f, -165.0f, 0.0f), Vec3(2828.0f, 2481.0f, 0.0f), Vec2(0.16f, 0.06f))),
			CursorUIData(UiData(L"GameTitle", Vec3(0.0f,  200.0f, 0.0f), Vec3(1036.0f,  106.0f, 0.0f), Vec2(1.0f,  1.0f))),
			CursorUIData(UiData(L"GameStart", Vec3(0.0f, -140.0f, 0.0f), Vec3(360.0f,   52.0f, 0.0f), Vec2(1.0f,  1.0f)),[&]() {
				m_sceneflag = true;
				SoundManager::GetInstance(GetThis<Stage>())->PlayOneShot(SoundManager::ID::EnterSE, 0.3f);
				m_FU->FadeIn(); }, true),
			//CursorUIData(UiData(L"Option"	, Vec3(0.0f, -200.0f, 0.0f), Vec3( 256.0f,   64.0f, 0.0f), Vec2(1.0f,  1.0f))),
			CursorUIData(UiData(L"Exit",	  Vec3(0.0f, -200.0f, 0.0f), Vec3(127.0f,   49.0f, 0.0f), Vec2(1.0f,  1.0f)),[]() {}, true)
		};

		for (auto data : titleDatas)
		{
			m_CursorUi->AddUiTexture(data); //UIの配置
		}
		m_CursorUi->SetCursorPos(0);
		m_CursorUi->SetAllDrawLayer(8); //UIのレイヤーの設定
		m_CursorUi->SetDrawLayer(10);

		// フェードイン及びフェードアウトの追加
		m_FU = AddGameObject<FadeUI>();
	}

	void TitleStage::OnUpdate()
	{
		// アプリの参照取得
		auto& app = App::GetApp();

		static const auto& inputDevice = app->GetMyInputDevice();
		static const auto& keyBoard = inputDevice->GetKeyBoard();

		// デバイスの取得
		auto device = app->GetInputDevice();
		const auto& pad = device.GetControlerVec()[0];

		// スティックの傾きをベクトルに変更
		Vec3 LeftStick(pad.fThumbLX, 0.0f, pad.fThumbLY);

		//コマンドの実行
		if (!m_sceneflag && pad.wPressedButtons & XINPUT_GAMEPAD_A && m_FU->GetColw() <= 0.0f)
		{
			m_CursorUi->Select();
		}

		if (m_FU->GetColw() >= 1.0f)
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(),
				App::GetApp()->GetScene<Scene>(), L"ToStageSelect");
			//SEStop
			SoundManager::GetInstance()->Stop(SoundManager::ID::TitleBGM);
		}

		//カーソルの移動
		if (pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_UP)
		{
			m_CursorUi->MoveCursor(-1);
			const float Volume = 0.15f;
			SoundManager::GetInstance(GetThis<Stage>())->PlayOneShot(SoundManager::ID::CursorSE, Volume);
		}
		if (pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			m_CursorUi->MoveCursor(+1);
			const float Volume = 0.15f;
			SoundManager::GetInstance(GetThis<Stage>())->PlayOneShot(SoundManager::ID::CursorSE, Volume);
		}

	}
}
//end basecross