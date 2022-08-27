/*!
@file GameStageBase.cpp
@brief GameStageBase実体
*/

#include "stdafx.h"
#include "Project.h"

#include "GameStageBase.h"
#include "PlayerInputer.h"

#include "PlayerObject.h"
#include "VillainPlayerObject.h"
#include "HeroPlayerObject.h"
#include "GameManagerObject.h"
#include "SlowTimerManager.h"

#include "MainCamera.h"
#include "MainCameraObject.h"
#include "StartCamera.h"

#include "StageMapCSV.h"
#include "WallObject.h"
#include "OriginalMeshStageObject.h"
#include "FixedBox.h"

#include "SingletonComponent.h"
#include "ImpactMap.h"

#include "UtilityStartCamera.h"

#include "EnemyObjectBase.h"

#include "OriginalMeshToonObject.h"
#include "ToonPNTStaticModelDraw.h"

#include "FieldImpactMap.h"

#include "ItemAcquisitionManager.h"

#include "HidePlace.h"
#include "HideItemObject.h"

#include "Watanabe/StageObject/StageObjects.h"
#include "Watanabe/UI/UIObjects.h"

#include "Itabashi/OnlineTestRoom.h"
#include "Itabashi/OnlinePlayerManager.h"

#include "Maruyama/StageObject/GoalObject.h"
#include "Itabashi/GamePlayerManager.h"

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
		//AddGameObject<GameObject>()->AddComponent<maru::FieldImpactMap>(maru::Utility::ConvertArrayType<GameObject>(m_floors));
		//外側コリジョン設定
		//CreateMapOutCollisions(m_floors);
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

	GameStageBase::GameStageBase() :
		Stage()
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