#include "stdafx.h"
#include "OnlineMatchStage.h"
#include "Patch/PlayerInputer.h"
#include "Scene.h"
#include "OnlineMatching.h"
#include "MatchStageTransitioner.h"
#include "Watanabe/DebugClass/Debug.h"
#include "Watanabe/UI/UIObjects.h"
#include "Watanabe/Component/MatchingUIController.h"
#include "Watanabe/Component/MatchingSyncPlayerObject.h"
#include "Watanabe/Effekseer/EfkEffect.h"
#include "Watanabe/Manager/PointManager.h"
#include "Watanabe/Manager/TimeManager.h"
#include "Watanabe/Manager/ScoreManager.h"
#include "Watanabe/UI/PasscodeUI.h"
#include "MatchingSelectUIObject.h"
#include "PasswordTextUI.h"
#include "UICancelEventer.h"
#include "MatchStageCoreObject.h"
#include "MatchStageUIObject.h"

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

		CreateMap(L"WaitStage.csv");

		auto matchStageCoreObject = AddGameObject<StageObject::MatchStageCoreObject>();
		auto onlineMatching = matchStageCoreObject->GetComponent<Online::OnlineMatching>();
		auto matchStageTransitioner = matchStageCoreObject->GetComponent<MatchStageTransitioner>();

		// UIの生成
		auto uiBuilder = CreateUI(L"MatchingUILayout.csv");

		auto matchStageUIObject = AddGameObject<StageObject::MatchStageUIObject>(uiBuilder);
		auto matchingUIController = matchStageUIObject->GetComponent<MatchingUIController>();
		matchingUIController->SetStageTransitioner(matchStageTransitioner);
		matchingUIController->SetOnlineMatching(onlineMatching);
		onlineMatching->SetMatchingUIController(matchingUIController);

		std::weak_ptr<MatchingSyncPlayerObject> weakMatchingSyncPlayerObject = matchStageCoreObject->GetComponent<MatchingSyncPlayerObject>();

		SimpleSoundManager::ChangeBGM(L"MatchingStageBGM", 0.1f);

		auto passwordText = matchStageUIObject->GetPasscodeUIObject()->GetComponent<UI::PasswordTextUI>();

		auto matchingSelectUIObject = matchStageUIObject->GetMatchingSelectUIObject();

		auto freeMatchingButton = matchingSelectUIObject->GetFreeMatchingButtonObject()->GetComponent<Button>();
		auto createRoomButton = matchingSelectUIObject->GetCreateRoomButtonObject()->GetComponent<Button>();
		auto joinRoomButton = matchingSelectUIObject->GetJoinRoomButtonObject()->GetComponent<Button>();

		std::weak_ptr<Online::OnlineMatching> weakOnlineMatching = onlineMatching;

		matchStageUIObject->GetComponent<UICancelEventer>()->AddCancelEvent([weakOnlineMatching, weakMatchingSyncPlayerObject]()
			{
				auto onlineMatching = weakOnlineMatching.lock();
				onlineMatching->StartLeaveRoom();

				EventSystem::GetInstance(onlineMatching->GetStage())->PopSelectableObject();

				weakMatchingSyncPlayerObject.lock()->Reset();
			}
		);

		freeMatchingButton->AddPushEvent([weakOnlineMatching]()
			{
				auto onlineMatching = weakOnlineMatching.lock();

				onlineMatching->StartFreeMatching();
			}
		);

		createRoomButton->AddPushEvent([weakOnlineMatching]()
			{
				auto onlineMatching = weakOnlineMatching.lock();

				onlineMatching->StartCreatePasswordMatching();
			}
		);

		passwordText->AddSubmitEvent([weakOnlineMatching](const std::wstring& password)
			{
				auto onlineMatching = weakOnlineMatching.lock();
				onlineMatching->StartJoinPasswordMatching(password);
			}
		);

		EventSystem::GetInstance(GetThis<Stage>())->SetBasicInputer(PlayerInputer::GetInstance());
	}

	void OnlineMatchStage::OnUpdate()
	{
	}
}