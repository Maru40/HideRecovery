/*!
@file GameStageBase.cpp
@brief GameStageBase実体
*/

#include "stdafx.h"
#include "Project.h"

#include "GameStageBase.h"
#include "PlayerInputer.h"

#include "PlayerObject.h"
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

namespace basecross {

	void GameStageBase::CreateMainCamera()
	{
		const Vec3 eye(0.0f, 5.0f, -5.0f);
		const Vec3 at(0.0f);

		//メインカメラの実装
		m_mainView = ObjectFactory::Create<SingleView>(GetThis<Stage>());
		m_mainCamera = ObjectFactory::Create<MainCamera>(m_player.GetShard());
		m_mainView->SetCamera(m_mainCamera);
		m_mainCamera->SetEye(eye);
		m_mainCamera->SetAt(at);
		m_mainCameraObject = AddGameObject<MainCameraObject>(m_player.GetShard());
		auto mainTrans = m_mainCameraObject->GetComponent<Transform>();
		mainTrans->SetPosition(eye);
		m_mainCameraObject->SetUpdateActive(false); 
	}

	void GameStageBase::CreateStartCamera(const wstring& stageName) {
		m_startView = ObjectFactory::Create<SingleView>(GetThis<Stage>());

		auto startCam = ObjectFactory::Create<StartCamera>(UtilityStartCamera::GetStartCameraShowParams(stageName));
		m_startView->SetCamera(startCam);
	}

	void GameStageBase::CreateMap(const wstring& fileName, const Vec3& offset) {
		auto map = AddGameObject<StageMapCSV>(L"MapDatas/", fileName);

		//床生成
		auto floors = map->CreateObject<FixedBox>(L"Floor", offset);
		map->CreateObject<WallObject>(L"Block", offset);
		vector<wstring> wallObjectNames = {
			L"OutWall", L"UpperWall", L"LowerWall", L"RoomBorderWall"
		};
		auto wallObjets = map->CreateObjects<WallObject>(wallObjectNames, offset);
		
		//PNTStaticDrawのオリジナルメッシュのオブジェクトの生成
		vector<wstring> originalMeshStageObjectNames = {
			L"rack" //L"tree_2", L"tree_dead3"
		};
		auto objects = map->CreateObjects<OriginalMeshStageObject<BcPNTStaticModelDraw>>(originalMeshStageObjectNames, offset);
		
		//トゥーンオブジェクトの生成
		vector<wstring> originalMeshToonObjects = {
			L"rock", L"rock001", L"rock005", L"rock007",
		};
		//auto toonObjects = map->CreateObjects<OriginalMeshToonObject>(originalMeshToonObjects, offset);
		auto toonObjects = map->CreateObjects<OriginalMeshStageObject<ToonPNTStaticModelDraw>>(originalMeshToonObjects, offset);

		//フィールドの影響マップの生成
		AddGameObject<GameObject>()->AddComponent<maru::FieldImpactMap>(maru::Utility::ConvertArrayType<GameObject>(floors));

		//外側コリジョン設定
		CreateMapOutCollisions(floors);
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
			EventSystem::GetInstance(GetThis<Stage>())->SetBasicInputer(PlayerInputer::GetInstance());

			AddGameObject<GameManagerObject>();
			m_player = Instantiate<PlayerObject>(Vec3(0.0f, 1.0f, 0.0f), Quat::Identity());
		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	std::shared_ptr<PlayerObject> GameStageBase::GetPlayer() const{
		return m_player.GetShard();
	}

	std::shared_ptr<MainCamera> GameStageBase::ChangeMainCamera(){
		auto camera = m_mainView->GetCamera();
		if (camera) {
			camera->SetCameraObject(m_mainCameraObject);
		}

		SetView(m_mainView);

		m_mainCameraObject->SetUpdateActive(true);
		return dynamic_pointer_cast<MainCamera>(camera);
	}

	std::shared_ptr<MainCameraObject> GameStageBase::GetMainCameraObject() const {
		return m_mainCameraObject;
	}

	std::shared_ptr<StartCamera> GameStageBase::ChangeStartCamera() {
		auto camera = m_startView->GetCamera();
		SetView(m_startView);

		auto startCamera = dynamic_pointer_cast<StartCamera>(camera);
		return startCamera;
	}
}

//endbasecross