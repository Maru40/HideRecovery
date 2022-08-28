#include "stdafx.h"
#include "MatchingUILayoutStage.h"
#include "../DebugClass/Debug.h"
#include "InputHelper.h"
#include "Scene.h"
#include "PlayerInputer.h"
#include "../UI/UIObjects.h"
#include "../Manager/PointManager.h"
#include "../Manager/ScoreManager.h"
#include "../BoardPoly/HPGaugeBP.h"
#include "../StageObject/StageObjects.h"
#include "Maruyama/Player/Component/PlayerSpawnPoint.h"
#include "VillainPlayerObject.h"

namespace basecross {
	void MatchingUILayoutStage::CreateViewLight() {
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
	void MatchingUILayoutStage::OnCreate() {
		CreateViewLight();
		AddGameObject<Debug>();
		Debug::GetInstance()->Log(L"MatchingUILayoutStage");
		ScoreManager::GetInstance()->AddKillCount(0);

		CreateMap(L"WaitStage.csv");
		PointManager::GetInstance()->AddPoint(team::TeamType::Blue);
		auto uiBuilder = CreateUI(L"ResultUILayout.csv");
		auto WinOrLose = uiBuilder->GetUIObject<WinOrLoseUI>(L"WinOrLose");
		WinOrLose->SetTeam(team::TeamType::Blue);

		//for (auto gameObject : GetGameObjectVec()) {
		//	auto playerSpawnPoint = dynamic_pointer_cast<PlayerSpawnPointObject>(gameObject);
		//	if (playerSpawnPoint)
		//	{
		//		auto player = AddGameObject<VillainPlayerObject>();
		//		player->GetComponent<Transform>()->SetPosition(playerSpawnPoint->GetComponent<Transform>()->GetWorldPosition());
		//	}
		//}
	}
	void MatchingUILayoutStage::OnUpdate() {
		const auto& inputDevice = App::GetApp()->GetMyInputDevice();
		const auto& pad = inputDevice->GetXInputGamePad();
		if (pad.IsInputDown(XInputCode::Start)) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToWatanabeStage");
		}
		if (pad.IsInputDown(XInputCode::A)) {
			//m_sprite->SetMessage(SplashMessageUI::MessageType::GetBall);
		}
	}
}