#include"LoadStage.h"
#include"Scene.h"
#include"SoundHelper.h"
#include"GameSaveManager.h"
#include"PlayerInputer.h"
#include "Watanabe/Effekseer/EfkEffect.h"
#include "Watanabe/Utility/CSVLoad.h"
#include "Watanabe/DebugClass/Debug.h"
#include "Watanabe/UI/UIObjects.h"
#include "Watanabe/StageObject/StageObjects.h"

namespace basecross
{
	LoadStage::ResourceLoadData::ResourceLoadData(const std::wstring& resourceKey, const std::wstring& filename) :
		resourceKey(resourceKey),
		filename(filename)
	{
	}

	LoadStage::ModelResourceLoadData::ModelResourceLoadData(const std::wstring& resourceKey, const std::wstring& filename,
		const ModelType modelType, const std::wstring& modelDir) :
		ResourceLoadData(resourceKey, filename),
		modelType(modelType),
		modelDir(modelDir)
	{
	}

	std::mutex LoadStage::m_mtx;
	bool LoadStage::m_isLoadFinish = false;

	const std::vector<LoadStage::ResourceLoadData> LoadStage::m_texureResourceLoadDatas =
	{
		ResourceLoadData(L"Floor_TX", L"floorRock_TX.png"),
		ResourceLoadData(L"Wall01_TX", L"Wall01_TX.png"),
		ResourceLoadData(L"Numbers_TX",L"Numbers_TX.png"),
		ResourceLoadData(L"StartAndFinish_TX",L"StartAndFinish_TX.png"),
		ResourceLoadData(L"Message_TX",L"Message_TX.png"),
		ResourceLoadData(L"Team_TX",L"Team_TX.png"),
		ResourceLoadData(L"HP_TX",L"HP_TX.png"),
		ResourceLoadData(L"HPGauge_TX",L"HPGauge_TX.png"),
		ResourceLoadData(L"HPColorGauge_TX",L"HPColorGauge_TX.png"),
		ResourceLoadData(L"TitleLogo_TX",L"TitleLogo_TX.png"),
		ResourceLoadData(L"WinOrLose_TX",L"WinOrLose_TX.png"),
		ResourceLoadData(L"ScoreLabel_TX",L"ScoreLabel_TX.png"),
		ResourceLoadData(L"Frame_TX",L"Frame_TX.png"),
		ResourceLoadData(L"Shape_TX",L"Shape_TX.png"),
		ResourceLoadData(L"Objective_TX",L"Objective_TX.png"),
		ResourceLoadData(L"RemainingTime_TX",L"RemainingTime_TX.png"),
		ResourceLoadData(L"CircleGauge_TX",L"CircleGauge_TX.png"),

		ResourceLoadData(L"FiledMap_TX", L"FiledMap_TX.png"),

		ResourceLoadData(L"Cursor_TX", L"Cursor.png"),
		ResourceLoadData(L"Reticle_TX", L"Reticle.png"),
		ResourceLoadData(L"ReticleCnterRed_TX", L"ReticleCtrR.png"),

		ResourceLoadData(L"RedGoal_TX", L"RedGoalTx.png"),
		ResourceLoadData(L"GoalTx", L"GoalTx.png"),

		ResourceLoadData(L"MapCursorUnderCircle_TX", L"MapCursorUnderCircle.png"),

		ResourceLoadData(L"TeleportUI_TX", L"TeleportUI_TX.png"),
		ResourceLoadData(L"Point_TX", L"Point.png"),

		ResourceLoadData(L"AButton_TX", L"AButton.png"),

		//RampTex
		ResourceLoadData(L"Ramp_Rock_TX", L"RampTex_Rock.png"),

		// コンテナ用テクスチャ
		ResourceLoadData(L"RedContianerTx", L"Contianer/RedContianerTx.png"),
		ResourceLoadData(L"GreenContianerTx", L"Contianer/GreenContianerTx.png"),
		ResourceLoadData(L"BlueContianerTx", L"Contianer/BlueContianerTx.png"),
		ResourceLoadData(L"YellowContianerTx", L"Contianer/YellowContianerTx.png"),

		// プレイヤーのテクスチャ
		ResourceLoadData(L"BlueHeroTx", L"Player/BlueHeroTx.png"),
		ResourceLoadData(L"RedHeroTx", L"Player/RedHeroTx.png"),

		//デバッグテクスチャ
		ResourceLoadData(L"Number_TX", L"Number.png"),
		ResourceLoadData(L"NumberWhite_TX", L"Number_White.png"),

		ResourceLoadData(L"Arrow_TX", L"Arrow.png"),
		ResourceLoadData(L"SquareSprite_TX", L"SquareSprite.png"),

		ResourceLoadData(L"Open_TX", L"Open_TX.png"),

		ResourceLoadData(L"FieldMapBox_TX", L"FiledMapBox_TX.png"),
	};

	const std::vector<LoadStage::ResourceLoadData> LoadStage::m_seResourceLoadDatas =

	{
		ResourceLoadData(L"Shot_SE", L"ShotSE.wav"),

		ResourceLoadData(L"ReadyArmsSE", L"ReadyArmsSE.wav"),
		ResourceLoadData(L"PlayerDamageSE", L"PlayerDamageSE.wav"),
		ResourceLoadData(L"DecisionSE", L"DecisionSE.wav"),
		ResourceLoadData(L"TitlePlessedSE", L"TitlePlessedSE.wav"),
		ResourceLoadData(L"GameStartSE", L"GameStartSE.wav"),
		ResourceLoadData(L"GoalEffectSE", L"GoalEffectSE.wav"),
		ResourceLoadData(L"StartCountSE", L"StartCountSE.wav"),

		ResourceLoadData(L"GoalBallSE", L"GoalBallSE.wav"),
		ResourceLoadData(L"GoalStartSE", L"GoalStartSe.wav"),
		ResourceLoadData(L"GetBallSE", L"GetBallSE.wav"),
		ResourceLoadData(L"StolenBallSE", L"StolenBallSE.wav"),
		ResourceLoadData(L"RelocationBallSE", L"RelocationBallSE.wav"),	//ボール再配置
		ResourceLoadData(L"TeleportSE", L"TeleportSE.wav"),
		ResourceLoadData(L"GameSetSE", L"GameSetSE.wav"),
		ResourceLoadData(L"AlertSE_00", L"AlertSe00.wav"),

		ResourceLoadData(L"OpenBoxSE", L"OpenBoxSe.wav"),
	};

	const std::vector<LoadStage::ResourceLoadData> LoadStage::m_bgmResourceLoadDatas =
	{
		ResourceLoadData(L"GameStageBGM", L"GameStageBGM.wav"),
		ResourceLoadData(L"TitleStageBGM", L"TitleStageBGM.wav"),
		ResourceLoadData(L"MatchingStageBGM", L"MatchingStageBGM.wav")
	};

	const std::vector<LoadStage::ModelResourceLoadData> LoadStage::m_modelResourceLoadDatas =
	{
		ModelResourceLoadData(L"Player_Mesh",  L"Hero.bmf", ModelType::BoneMulti,  L"Player\\"),

		ModelResourceLoadData(L"Ball_Model",  L"Ball.bmf", ModelType::BoneMulti,  L"Ball\\"),
		ModelResourceLoadData(L"Box_Model", L"Box.bmf", ModelType::BoneMulti, L"Box\\"),

		ModelResourceLoadData(L"SkyBox_Model", L"SkyBox.bmf", ModelType::Static, L"SkyBox\\"),

		ModelResourceLoadData(L"rack",  L"tana.bmf", ModelType::Static,  L"StageObject\\"),
		ModelResourceLoadData(L"Container",  L"Container.bmf", ModelType::StaticMulti,  L"StageObject\\"),
		ModelResourceLoadData(L"Goal",  L"Goal.bmf", ModelType::StaticMulti,  L"StageObject\\"),
	};

	LoadStage::LoadStage(const std::wstring& nextStageKey) :
		m_nextStageKey(nextStageKey)
	{ }

	void LoadStage::CreateViewLight()
	{
		const Vec3 eye(0.0f, 10.0f, 20.0f);
		const Vec3 at(3.0f);
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

	void LoadStage::LoadResource()
	{
		std::lock_guard<std::mutex> lock(m_mtx);

		const auto& app = App::GetApp();

		std::wstring mediaDir;
		mediaDir = app->GetDataDirWString();

		// テクスチャフォルダ

		std::wstring dir = mediaDir + L"Textures\\";

		for (auto& textureLoadData : m_texureResourceLoadDatas)
		{
			app->RegisterTexture(textureLoadData.resourceKey, dir + textureLoadData.filename);
		}

		// SEフォルダ

		dir = mediaDir + L"SEs\\";

		for (auto& seLoadData : m_seResourceLoadDatas)
		{
			app->RegisterWav(seLoadData.resourceKey, dir + seLoadData.filename);
		}

		// BGMフォルダ

		dir = mediaDir + L"BGMs\\";

		for (auto& bgmLoadData : m_bgmResourceLoadDatas)
		{
			app->RegisterWav(bgmLoadData.resourceKey, dir + bgmLoadData.filename);
		}

		// モデルフォルダ

		dir = mediaDir + L"Models\\";

		for (auto& modelLoadData : m_modelResourceLoadDatas)
		{
			std::shared_ptr<MeshResource> modelMesh = nullptr;
			std::shared_ptr<MultiMeshResource> modelMultiMesh = nullptr;

			std::wstring modelDir = dir + modelLoadData.modelDir;

			switch (modelLoadData.modelType)
			{
			case ModelType::Static:
				modelMesh = MeshResource::CreateStaticModelMesh(modelDir, modelLoadData.filename);
				break;

			case ModelType::Bone:
				modelMesh = MeshResource::CreateBoneModelMesh(modelDir, modelLoadData.filename);
				break;

			case ModelType::StaticMulti:
				modelMultiMesh = MultiMeshResource::CreateStaticModelMultiMesh(modelDir, modelLoadData.filename);
				break;

			case ModelType::BoneMulti:
				modelMultiMesh = MultiMeshResource::CreateBoneModelMultiMesh(modelDir, modelLoadData.filename);
				break;
			}

			if (modelMesh) {
				app->RegisterResource(modelLoadData.resourceKey, modelMesh);
			}
			else {
				app->RegisterResource(modelLoadData.resourceKey, modelMultiMesh);
			}
		}

		auto& soundSetting = GameSaveManager::GetSoundSetting();
		if (!soundSetting->Load())
		{
			soundSetting->SetBGMVolume(1.0f);
			soundSetting->SetSEVolume(1.0f);
		}

		m_isLoadFinish = true;
	}

	void LoadStage::LoadEffectDatas() {
		// エフェクトの登録に必要
		AddGameObject<EfkInterface>();

		const auto& app = App::GetApp();
		std::wstring dir = app->GetDataDirWString() + L"Effects/";
		EfkEffectResource::RegisterEffectResource(L"Fireworks", dir + L"Fireworks.efk");
		EfkEffectResource::RegisterEffectResource(L"Confetti", dir + L"Confetti.efk");
		EfkEffectResource::RegisterEffectResource(L"Respawn", dir + L"Respawn.efk");
		EfkEffectResource::RegisterEffectResource(L"Smoke", dir + L"Smoke.efk");
		EfkEffectResource::RegisterEffectResource(L"PlayerHit", dir + L"PlayerHit.efk");
		EfkEffectResource::RegisterEffectResource(L"BlueHit", dir + L"BlueHit.efk");
		EfkEffectResource::RegisterEffectResource(L"RedHit", dir + L"RedHit.efk");
		EfkEffectResource::RegisterEffectResource(L"MuzzleFlash", dir + L"MuzzleFlash.efk");
		EfkEffectResource::RegisterEffectResource(L"BlueBullet", dir + L"BlueBullet.efk");
		EfkEffectResource::RegisterEffectResource(L"RedBullet", dir + L"RedBullet.efk");
		EfkEffectResource::RegisterEffectResource(L"Tackle", dir + L"Tackle.efk");
		EfkEffectResource::RegisterEffectResource(L"HasBall", dir + L"HasBall.efk");
	}

	void LoadStage::OnCreate()
	{
		itbs::Input::CursorSetting::SetIsLock(true);
		itbs::Input::CursorSetting::SetIsDraw(false);

		CreateViewLight();

		AddGameObject<Debug>();
		Debug::GetInstance()->Log(L"LoadStage");

		// ローディングテクスチャの読み込み
		const auto& app = App::GetApp();
		wstring mediaDir = app->GetDataDirWString();
		wstring dir = mediaDir + L"Textures/";
		app->RegisterTexture(L"Loading_TX", dir + L"Loading_TX.png");
		app->RegisterTexture(L"LoadingBG_TX", dir + L"LoadingBG_TX.png");

		// 「Now Loading...」の表示
		auto uiBuilder = UIObjectCSVBuilder::Create();
		uiBuilder->Register<SimpleSprite>(L"SimpleSprite");
		auto path = mediaDir + L"MapDatas/";
		uiBuilder->Build(GetThis<Stage>(), path + L"LoadUILayout.csv");

		m_stageBackColor = App::GetApp()->GetScene<Scene>()->GetClearColor();

		App::GetApp()->GetScene<Scene>()->SetClearColor(Col4(0, 0, 0, 1));

		LoadEffectDatas();

		std::thread loadThread(LoadResource);

		loadThread.detach();

		// CSVファイルの読み込み（できれば関数で分けたい）
		dir = mediaDir + L"Models/";
		CSVLoad::GetInstance()->RegisterFile(L"PlayerAnimation", dir + L"Player/PlayerAnimation.csv");
		CSVLoad::GetInstance()->RegisterFile(L"BallAnimation", dir + L"Ball/BallAnimation.csv");
		CSVLoad::GetInstance()->RegisterFile(L"BoxAnimation", dir + L"Box/BoxAnimation.csv");
		dir = mediaDir + L"CSVDatas/";
		CSVLoad::GetInstance()->RegisterFile(L"SpriteData", dir + L"SpriteData.csv");
	}

	void LoadStage::OnUpdate()
	{
		if (m_isLoadFinish)
		{
			App::GetApp()->GetScene<Scene>()->SetClearColor(m_stageBackColor);
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), m_nextStageKey);
		}
	}
}