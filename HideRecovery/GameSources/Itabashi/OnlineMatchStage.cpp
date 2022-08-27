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

namespace basecross
{
	void OnlineMatchStage::OnCreate()
	{
		const Vec3 eye(0.0f, 3.0f, 20.0f);
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

		auto debugObject = AddGameObject<Debug>();
		debugObject->Log(L"OnlineMatchStage");

		auto gameObject = AddGameObject<GameObject>();

		auto onlineMatching = gameObject->AddComponent<Online::OnlineMatching>();
		auto matchStageRoomer = gameObject->AddComponent<MatchStageRoomer>();
		auto matchStageTransitioner = gameObject->AddComponent<MatchStageTransitioner>();

		// マップとUIの生成
		CreateMap(L"WaitStage.csv");
		auto uiBuilder = CreateUI(L"MatchingUILayout.csv");
		gameObject->AddComponent<MatchingUIController>(uiBuilder);

		SimpleSoundManager::ChangeBGM(L"MatchingStageBGM", 0.1f);
	}

	void OnlineMatchStage::OnUpdate()
	{
	}
}