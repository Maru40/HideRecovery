﻿/*!
@file GameStageBase.cpp
@brief GameStageBase実体
*/

#include "stdafx.h"
#include "Project.h"

#include "GameStageBase.h"
#include "Patch/PlayerInputer.h"

#include "Maruyama/Player/Object/PlayerObject.h"
#include "Maruyama/Player/Object/VillainPlayerObject.h"
#include "Maruyama/Player/Object/HeroPlayerObject.h"
#include "Maruyama/Utility/SingletonComponent/GameManager.h"
#include "Maruyama/Utility/SingletonComponent/SlowTimerManager.h"

#include "Maruyama/Camera/MainCamera.h"
#include "Maruyama/Camera/MainCameraObject.h"
#include "Maruyama/Camera/StartCamera.h"

#include "Maruyama/Load/StageMapCSV.h"
#include "Maruyama/StageObject/OriginalMeshStageObject.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Enemy/ImpactMap/ImpactMap.h"

#include "Maruyama/Camera/UtilityStartCamera.h"

#include "Maruyama/Enemy/Object/EnemyObjectBase.h"

#include "Maruyama/StageObject/OriginalMeshToonObject.h"
#include "Maruyama/Shader/Toon/ToonPNTStaticModelDraw.h"

#include "Maruyama/Enemy/ImpactMap/FieldImpactMap.h"

#include "Maruyama/Player/Component/ItemAcquisitionManager.h"

#include "Maruyama/StageObject/HidePlace.h"
#include "Maruyama/Item/HideItemObject.h"

#include "Watanabe/StageObject/StageObjects.h"
#include "Watanabe/UI/UIObjects.h"

#include "Itabashi/OnlineTestRoom.h"

#include "Maruyama/StageObject/GoalObject.h"
#include "Itabashi/GamePlayerManager.h"

#include "Itabashi/OnlineStatus.h"

namespace basecross {
	void GameStageBase::CreateMainCamera()
	{
		//const Vec3 eye(0.0f, 5.0f, -5.0f);
		//const Vec3 at(0.0f);

		////メインカメラの実装
		//m_mainView = ObjectFactory::Create<SingleView>(GetThis<Stage>());
		//m_mainCamera = ObjectFactory::Create<MainCamera>(m_player.GetShard());
		//m_mainView->SetCamera(m_mainCamera);
		//m_mainCamera->SetEye(eye);
		//m_mainCamera->SetAt(at);
		//m_mainCameraObject = AddGameObject<MainCameraObject>(m_player.GetShard());
		//auto mainTrans = m_mainCameraObject->GetComponent<Transform>();
		//mainTrans->SetPosition(eye);
		//m_mainCameraObject->SetUpdateActive(false);

		const Vec3 eye(0.0f, +15.0f, -30.0f);
		const Vec3 at(0.0f);
		m_mainView = CreateView<SingleView>();
		//ビューのカメラの設定
		m_mainCamera = ObjectFactory::Create<Camera>();
		m_mainView->SetCamera(m_mainCamera);
		m_mainCamera->SetEye(eye);
		m_mainCamera->SetAt(at);
	}

	void GameStageBase::CreateStartCamera(const wstring& stageName) {
		m_startView = ObjectFactory::Create<SingleView>(GetThis<Stage>());

		auto startCam = ObjectFactory::Create<StartCamera>(UtilityStartCamera::GetStartCameraShowParams(stageName));
		m_startView->SetCamera(startCam);
	}

	void GameStageBase::CreateMap(const wstring& fileName) {
		GameObjecttCSVBuilder builder;
		builder.Register<Block>(L"Block");
		builder.Register<RackObject>(L"Rack");
		builder.Register<HideItemObject>(L"HideItem");
		builder.Register<PlayerSpawnPointObject>(L"PlayerSpawnPoint");
		builder.Register<GoalObject>(L"Goal");
		builder.Register<OwnAreaObject>(L"OwnAreaObject");
		builder.Register<Container>(L"Container");
		auto dir = App::GetApp()->GetDataDirWString();
		auto path = dir + L"MapDatas/";
		builder.Build(GetThis<Stage>(), path + fileName);

		AddGameObject<SkyBox>(Vec3(500));

		//フィールドの影響マップの生成
		for (auto& block : maru::Utility::FindGameObjects<Block>(GetThis<Stage>())) {
			if (block->GetName() == L"Floor") {	//床の取得
				m_floors.push_back(block);
				break;
			}
		}
		AddGameObject<GameObject>()->AddComponent<maru::FieldImpactMap>(maru::Utility::ConvertArrayType<GameObject>(m_floors));
		//外側コリジョン設定
		//CreateMapOutCollisions(m_floors);

		UniqueIdCreater<std::uint32_t> idCreater(m_stageObjectIdSeed, 1, UINT_MAX);

		for (auto& onlineStatus : maru::Utility::FindComponents<Online::OnlineStatus>(GetThis<Stage>()))
		{
			// 成功するまで繰り返す
			while (!onlineStatus->ChangeInstanceId(idCreater.CreateId())) {}
		}
	}

	std::shared_ptr<UIObjectCSVBuilder> GameStageBase::CreateUI(const wstring& fileName) {
		auto uiBuilder = UIObjectCSVBuilder::Create();
		uiBuilder->Register<TimerUI>(L"TimerUI");
		uiBuilder->Register<HPGaugeUI>(L"HPGaugeUI");
		uiBuilder->Register<PointUI>(L"PointUI");
		uiBuilder->Register<SimpleSprite>(L"SimpleSprite");
		uiBuilder->Register<SplashMessageUI>(L"SplashMessageUI");
		uiBuilder->Register<ScoreUI>(L"ScoreUI");
		uiBuilder->Register<WinOrLoseUI>(L"WinOrLoseUI");
		uiBuilder->Register<Numbers>(L"Numbers");
		uiBuilder->Register<ButtonUI>(L"ButtonUI");
		uiBuilder->Register<HoldButtonUI>(L"HoldButtonUI");
		uiBuilder->Register<InProcessUI>(L"InProcessUI");
		uiBuilder->Register<DirectionWithHasBallUI>(L"DirectionWithHasBallUI");
		auto dir = App::GetApp()->GetDataDirWString();
		auto path = dir + L"MapDatas/";
		uiBuilder->Build(GetThis<Stage>(), path + fileName);
		m_uiObjectCSVBuilder = uiBuilder;
		return uiBuilder;
	}

	void GameStageBase::CreateMapOutCollision(const Vec3& startPosition, const Vec3& forward, const float& length, const float& width, const float& height) {
		const float MoveForwardRange = width * 1.5f;
		auto halfHeight = height * 0.5f;
		auto direct = maru::Utility::ConvertForwardOffset(forward, Vec3::Right());
		auto position = startPosition + (direct.GetNormalized() * MoveForwardRange);
		position.y += halfHeight;
		auto object = Instantiate<GameObject>(position, Quat::Identity());
		auto collision = object->AddComponent<CollisionObb>();
		object->AddTag(L"T_Obstacle");

		constexpr float depth = 1.0f;
		auto objectTrans = object->GetComponent<Transform>();
		objectTrans->SetScale(Vec3(length - width, height, depth));
		objectTrans->SetForward(forward);

		collision->SetFixed(true);
		//collision->SetDrawActive(true);
	}

	void GameStageBase::RenderStage() {
		Stage::RenderStage();
	}

	GameStageBase::GameStageBase(std::uint64_t seed) :
		Stage(),
		m_stageObjectIdSeed(seed)
	{}

	void GameStageBase::OnCreate() {
		try {
		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	std::shared_ptr<PlayerObject> GameStageBase::GetPlayer() const {
		return m_player.GetShard();
	}

	std::shared_ptr<Camera> GameStageBase::ChangeMainCamera() {
		//auto camera = m_mainView->GetCamera();
		//if (camera) {
		//	camera->SetCameraObject(m_mainCameraObject);
		//}

		//SetView(m_mainView);

		//m_mainCameraObject->SetUpdateActive(true);
		//return dynamic_pointer_cast<MainCamera>(camera);

		auto camera = m_mainView->GetCamera();
		SetView(m_mainView);

		return camera;
	}

	//std::shared_ptr<MainCameraObject> GameStageBase::GetMainCameraObject() const {
	//	return m_mainCameraObject;
	//}

	std::shared_ptr<StartCamera> GameStageBase::ChangeStartCamera() {
		auto camera = m_startView->GetCamera();
		SetView(m_startView);

		auto startCamera = dynamic_pointer_cast<StartCamera>(camera);
		return startCamera;
	}
}

//endbasecross