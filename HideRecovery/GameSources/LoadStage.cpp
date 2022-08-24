#include"LoadStage.h"
#include"Scene.h"
#include"SoundHelper.h"
#include"GameSaveManager.h"
#include"PlayerInputer.h"
#include "Watanabe/Effekseer/EfkEffect.h"
#include "Watanabe/Utility/CSVLoad.h"
#include "Watanabe/DebugClass/Debug.h"

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
		ResourceLoadData(L"TitleLogo_TX",L"TitleLogo_TX.png"),
		ResourceLoadData(L"WinOrLose_TX",L"WinOrLose_TX.png"),

		//ResourceLoadData(L"gauge", L"Gauge.png"),
		//ResourceLoadData(L"clear", L"StageClear.png"),
		//ResourceLoadData(L"Circle_TX", L"Circle.png"),
		//ResourceLoadData(L"Rock_TX", L"RockTX.png"),
		//ResourceLoadData(L"Point_TX", L"Point.png"),
		//ResourceLoadData(L"Segment_TX", L"Segment.png"),

		//Effect
		//ResourceLoadData(L"Clear_TX", L"Clear_TX.png"),
		//ResourceLoadData(L"SquareSpriteAlpha_TX", L"SquareSpriteAlpha.png"),

		//StageSelect
		//ResourceLoadData(L"RB_TX", L"RB.png"),
		//ResourceLoadData(L"RBBack_TX", L"RBBack.png"),
		//ResourceLoadData(L"LB_TX", L"LB.png"),
		//ResourceLoadData(L"LBBack_TX", L"LBBack.png"),
		//ResourceLoadData(L"Stage_TX", L"Stage.png"),
		//ResourceLoadData(L"SelectBack_TX", L"SelectBack.png"),
		//ResourceLoadData(L"StageSelectMenuBack_TX", L"StageSelectMenuBack.png"),
		//ResourceLoadData(L"StageSelectMenuBack2_TX", L"StageSelectMenuBack2.png"),

		//BoostTX
		//ResourceLoadData(L"Boost_TX", L"Boost_TX.png"),
		//ResourceLoadData(L"LineH_TX", L"LineH.png"),
		//ResourceLoadData(L"LineV_TX", L"LineV.png"),
		//ResourceLoadData(L"Aura_TX", L"Aura_TX.png"),

		//Stage
		//ResourceLoadData(L"StageS1_TX", L"StageS1.png"),
		//ResourceLoadData(L"StageS2_TX", L"StageS1.png"),
		//ResourceLoadData(L"StageM1_TX", L"StageM1.png"),
		//ResourceLoadData(L"StageM2_TX", L"StageM2.png"),
		//ResourceLoadData(L"StageL1_TX", L"StageL1.png"),
		//ResourceLoadData(L"StageL2_TX", L"StageL2.png"),

		//RampTex
		ResourceLoadData(L"Ramp_Rock_TX", L"RampTex_Rock.png"),

		//デバッグテクスチャ
		ResourceLoadData(L"Number_TX", L"Number.png"),
		ResourceLoadData(L"NumberWhite_TX", L"Number_White.png"),

		ResourceLoadData(L"Arrow_TX", L"Arrow.png"),
		ResourceLoadData(L"SquareSprite_TX", L"SquareSprite.png"),
	};

	const std::vector<LoadStage::ResourceLoadData> LoadStage::m_seResourceLoadDatas =

	{
		//ResourceLoadData(L"HideSE", L"HideSE.wav"),
		//ResourceLoadData(L"SlimePressing_SE", L"Slime_Pressing.wav"),
		//ResourceLoadData(L"SlimePressed_SE", L"Slime_Pressed.wav"),
		//ResourceLoadData(L"Clear_SE", L"AS_ClearSE.wav"),
		//ResourceLoadData(L"Back_SE", L"AS_BackSE.wav"),
		//ResourceLoadData(L"Next_SE", L"AS_NextSE.wav"),
		//ResourceLoadData(L"CursorSE", L"AS_CursorSE.wav"),
		//ResourceLoadData(L"EnterSE", L"AS_CursorSelect.wav"),
		//ResourceLoadData(L"PlayerWalk", L"AS_PlayerWalk.wav"),
		//ResourceLoadData(L"PlayerTackle", L"AS_PlayerTackle.wav"),
		//ResourceLoadData(L"JettPomp", L"AS_JettPomp.wav"),
		ResourceLoadData(L"Shot_SE", L"AS_1086937_SF_ゲーム＿レーザー音＿ドチューン.wav"),
	};

	const std::vector<LoadStage::ResourceLoadData> LoadStage::m_bgmResourceLoadDatas =
	{
		//ResourceLoadData(L"Title_BGM_01", L"AS_SelectBGM1.wav"),
		//ResourceLoadData(L"Select_BGM_01", L"AS_SelectBGM1.wav"),
		//ResourceLoadData(L"Game_BGM_01", L"AS_GameBGM1.wav"),
		//ResourceLoadData(L"Title_BGM_01", L"AS_TitleBGM1.wav"),
	};

	const std::vector<LoadStage::ModelResourceLoadData> LoadStage::m_modelResourceLoadDatas =
	{
		//Handy
		//ModelResourceLoadData(L"Handy_Walk",  L"Handy_Walk2.bmf", ModelType::Bone,  L"Handy\\"),rock.001.bmf/tree_2

		//ModelResourceLoadData(L"rock",  L"rock.bmf", ModelType::Static,  L"Rook\\"),
		//ModelResourceLoadData(L"rock001",  L"rock001.bmf", ModelType::Static,  L"Rook\\"),
		//ModelResourceLoadData(L"rock005",  L"rock.005.bmf", ModelType::Static,  L"Rook\\"),
		//ModelResourceLoadData(L"rock007",  L"rock007.bmf", ModelType::Static,  L"Rook\\"),
		//ModelResourceLoadData(L"tree_2",  L"tree_2.bmf", ModelType::Static,  L"Tree\\"),
		//ModelResourceLoadData(L"tree_dead2",  L"tree_dead2.bmf", ModelType::Static,  L"Tree\\"),
		//ModelResourceLoadData(L"tree_dead3",  L"tree_dead3.bmf", ModelType::Static,  L"Tree\\"),

		//ModelResourceLoadData(L"Player_Mesh",  L"Character04.bmf", ModelType::BoneMulti,  L"Player\\"),
		ModelResourceLoadData(L"Player_Mesh",  L"Hero.bmf", ModelType::BoneMulti,  L"Player\\"),

		//ModelResourceLoadData(L"Player_Idle",  L"Player_Animation_Idol.bmf", ModelType::BoneMulti,  L"Player\\"),
		//ModelResourceLoadData(L"Player_WalkStart",  L"Player_Animation_Walk_Start.bmf", ModelType::BoneMulti,  L"Player\\"),
		//ModelResourceLoadData(L"Player_Walk",  L"Player_Animation_Walk.bmf", ModelType::BoneMulti,  L"Player\\"),
		//ModelResourceLoadData(L"Player_WalkEnd",  L"Player_Animation_Walk_End.bmf", ModelType::BoneMulti,  L"Player\\"),
		//ModelResourceLoadData(L"Player_Tackle",  L"Player_Animation_Tackle.bmf", ModelType::BoneMulti,  L"Player\\"),
		//ModelResourceLoadData(L"Player_TackleEnd",  L"Player_Animation_Tackle_End.bmf", ModelType::BoneMulti,  L"Player\\"),
		//ModelResourceLoadData(L"Player_Aim",  L"Player_Animation_Aim.bmf", ModelType::BoneMulti,  L"Player\\"),
		//ModelResourceLoadData(L"Player_Shot",  L"Player_Animation_Shot.bmf", ModelType::BoneMulti,  L"Player\\"),
		//ModelResourceLoadData(L"Player_Damage",  L"Player_Animation_Damage.bmf", ModelType::BoneMulti,  L"Player\\"),
		//ModelResourceLoadData(L"Player_Death",  L"Player_Animation_Dead.bmf", ModelType::BoneMulti,  L"Player\\"),
		//ModelResourceLoadData(L"Player_WinIdle",  L"Player_Animation_Win_Idle.bmf", ModelType::BoneMulti,  L"Player\\"),
		//ModelResourceLoadData(L"Player_WinMove",  L"Player_Animation_Win.bmf", ModelType::BoneMulti,  L"Player\\"),

		//ModelResourceLoadData(L"Player_Mesh",  L"Player_Mesh.bmf", ModelType::BoneMulti,  L"Player\\"),

		ModelResourceLoadData(L"rack",  L"tana.bmf", ModelType::Static,  L"StageObject\\"),
		ModelResourceLoadData(L"Goal",  L"Goal.bmf", ModelType::StaticMulti,  L"StageObject\\"),
	};

	LoadStage::LoadStage(const std::wstring& nextStageKey) :
		m_nextStageKey(nextStageKey)
	{ }

	void LoadStage::CreateViewLight()
	{
		const Vec3 eye(0.0f, 5.0f, -5.0f);
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
		EfkEffectResource::RegisterEffectResource(L"TestEffect", dir + L"TestEffect.efk");
		EfkEffectResource::RegisterEffectResource(L"Fireworks", dir + L"Fireworks.efk");
	}

	void LoadStage::OnCreate()
	{
		itbs::Input::CursorSetting::SetIsLock(true);
		itbs::Input::CursorSetting::SetIsDraw(false);

		CreateViewLight();

		AddGameObject<Debug>();
		Debug::GetInstance()->Log(L"LoadStage");

		m_stageBackColor = App::GetApp()->GetScene<Scene>()->GetClearColor();

		App::GetApp()->GetScene<Scene>()->SetClearColor(Col4(0, 0, 0, 1));

		LoadEffectDatas();

		std::thread loadThread(LoadResource);

		loadThread.detach();

		// CSVファイルの読み込み（できれば関数で分けたい）
		wstring mediaDir;
		mediaDir = App::GetApp()->GetDataDirWString();
		wstring dir = mediaDir + L"Models/";
		CSVLoad::GetInstance()->RegisterFile(L"PlayerAnimation", dir + L"Player/PlayerAnimation.csv");
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