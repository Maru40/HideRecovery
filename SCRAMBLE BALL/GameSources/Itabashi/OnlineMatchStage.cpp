#include "stdafx.h"
#include "OnlineMatchStage.h"
#include "PlayerInputer.h"
#include "Scene.h"
#include "OnlineMatching.h"
#include "MatchStageTransitioner.h"
#include "MatchStageRoomer.h"
#include "Watanabe/DebugClass/Debug.h"
#include "Watanabe/UI/UIObjects.h"
#include "Watanabe/Component/MatchingUIController.h"
#include "Watanabe/Component/MatchingSyncPlayerObject.h"
#include "Watanabe/Effekseer/EfkEffect.h"
#include "Watanabe/Manager/PointManager.h"
#include "Watanabe/Manager/TimeManager.h"
#include "Watanabe/Manager/ScoreManager.h"

namespace basecross
{
	void OnlineMatchStage::OnCreate()
	{
		const Vec3 eye(0.0f, 0.5f, 5.0f);
		const Vec3 at(0, 0.5f, 0);
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

		AddGameObject<EfkInterface>();
		auto debugObject = AddGameObject<Debug>();
		debugObject->Log(L"OnlineMatchStage");

		// 明示的にインスタンスを生成
		PointManager::CreateInstance();
		TimeManager::CreateInstance();
		ScoreManager::CreateInstance();

		auto gameObject = AddGameObject<GameObject>();
		auto onlineMatching = gameObject->AddComponent<Online::OnlineMatching>();
		auto matchStageRoomer = gameObject->AddComponent<MatchStageRoomer>();
		auto matchStageTransitioner = gameObject->AddComponent<MatchStageTransitioner>();

		// マップとUIの生成
		CreateMap(L"WaitStage.csv");
		auto uiBuilder = CreateUI(L"MatchingUILayout.csv");
		gameObject->AddComponent<MatchingUIController>(uiBuilder);

		// スポーンオブジェクトを取得
		vector<shared_ptr<PlayerSpawnPointObject>> spawnPointObjects;
		for (auto gameObject : GetGameObjectVec()) {
			if (auto spawnObject = dynamic_pointer_cast<PlayerSpawnPointObject>(gameObject)) {
				spawnPointObjects.push_back(spawnObject);
			}
		}
		// IDでソート
		sort(spawnPointObjects.begin(), spawnPointObjects.end(),
			[](const shared_ptr<PlayerSpawnPointObject>& a, const shared_ptr<PlayerSpawnPointObject>& b) {
				return a->GetID() < b->GetID();
			}
		);
		gameObject->AddComponent<MatchingSyncPlayerObject>(spawnPointObjects);

		SimpleSoundManager::ChangeBGM(L"MatchingStageBGM", 0.1f);
	}

	void OnlineMatchStage::OnUpdate()
	{
	}
}