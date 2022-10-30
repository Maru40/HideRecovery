#include "stdafx.h"
#include "Project.h"

#include "WatanabeStage.h"
#include "../DebugClass/Debug.h"
#include "../Effekseer/EfkEffect.h"
#include "../StageObject/Block.h"
#include "../StageObject/RackObject.h"
#include "../Shader/BoneModelDraw.h"
#include "Maruyama/Player/Object/HeroPlayerObject.h"
#include "Maruyama/Player/Object/VillainPlayerObject.h"
#include "Patch/CameraHelper.h"
#include "Itabashi/OnlinePlayerSynchronizer.h"
#include "Itabashi/OnlineTransformSynchronization.h"
#include "Patch/SpringArmComponent.h"
#include "Patch/CameraHelper.h"
#include "Maruyama/Camera/Component/LookAtCameraManager.h"
#include "../Component/TestComponent.h"
#include "Itabashi/ObjectMover.h"
#include "Maruyama/Utility/Component/RotationController.h"
#include "Patch/PlayerInputer.h"
#include "Maruyama/Utility/Utility.h"
#include "../Utility/CSVLoad.h"
#include "../Component/PlayerAnimator.h"
#include "../UI/Numbers.h"
#include "../UI/CountDownUI.h"
#include "../UI/SimpleSprite.h"
#include "../UI///GameStartUI.h"
#include "../Manager/PointManager.h"
#include "../Manager/TimeManager.h"
#include "../StageObject/FireworksObject.h"
#include "../UI/GoalMessageUI.h"
#include "../UI/GameFinishUI.h"
#include "../UI/TimerUI.h"
#include "../StageObject/OwnAreaObject.h"

#include "Maruyama/Item/HideItemObject.h"
#include "Maruyama/StageObject/GoalObject.h"

#include "../UI/HPGaugeUI.h"
#include "../UI/PointUI.h"
#include "../UI/UIObjectCSVBuilder.h"
#include "../StageObject/Container.h"

namespace basecross {
	void WatanabeStage::CreateViewLight() {
		const Vec3 eye(5.0f, 5.0f, -5.0f);
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

	void WatanabeStage::OnCreate() {
		CreateViewLight();
		AddGameObject<EfkInterface>();
		AddGameObject<Debug>();
		Debug::GetInstance()->Log(L"WatanabeStage");

		auto testObj = AddGameObject<GameObject>();
		auto efkComp = testObj->AddComponent<EfkComponent>();
		efkComp->SetEffectResource(L"TestEffect");
		//efkComp->PlayLoop(L"TestEffect");

		AddGameObject<CameraObject>();
		// ヴィランプレイヤーを生成
		auto player = Instantiate<VillainPlayerObject>(Vec3(0.0f, 1.0f, 0.0f), Quat::Identity());
		// テストのためオンライン系のコンポーネントを削除
		player->RemoveComponent<OnlinePlayerSynchronizer>();
		player->RemoveComponent<Online::OnlineTransformSynchronization>();
		// プレイヤーを見るようなカメラを設定
		auto sp = player->GetArm()->GetComponent<SpringArmComponent>();
		auto& tpsCamera = sp->GetChildObject();
		tpsCamera->AddComponent<VirtualCamera>(10);
		tpsCamera->AddComponent<LookAtCameraManager>(player, LookAtCameraManager::Parametor());

		auto stage = GetThis<WatanabeStage>();
		auto testComp = player->AddComponent<TestComponent>();
		// テスト用にプレイヤーの移動処理を実装
		testComp->SetOnUpdateFunction(
			[player, stage]() {
				auto objectMover = player->GetComponent<Operator::ObjectMover>();
				if (!objectMover)
					return;

				objectMover->Move(PlayerInputer::GetMoveDirection());

				auto rotationController = player->GetComponent<RotationController>();
				if (rotationController)
				{
					auto input = PlayerInputer::GetMoveDirection();
					auto direct = maru::Utility::CalcuCameraVec(Vec3(input.x, 0, input.y), stage->GetView()->GetTargetCamera(), player);

					rotationController->SetDirect(direct);
				}
			}
		);

		//{
		//	m_obj = AddGameObject<GameObject>();
		//	auto testDraw = m_obj->AddComponent<BoneModelDraw>();
		//	testDraw->SetMultiMeshResource(L"Player_Mesh");
		//	//testDraw->SetMeshResource(L"rack");
		//}
		//player->RemoveComponent<PlayerAnimationCtrl>();
		player->AddComponent<PlayerAnimator>();
		m_obj = player;

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
		builder.Build(GetThis<Stage>(), path + L"StageS2.csv");

		UIObjectCSVBuilder uiBuilder;
		uiBuilder.Register<TimerUI>(L"TimerUI");
		uiBuilder.Register<HPGaugeUI>(L"HPGaugeUI");
		uiBuilder.Register<PointUI>(L"PointUI");
		uiBuilder.Register<SimpleSprite>(L"SimpleSprite");
		uiBuilder.Build(GetThis<Stage>(), path + L"GameUILayout.csv");

		//AddGameObject<NumberSprite>()->SetValue(5);
		m_gameStartUI = AddGameObject<GameStartUI>();
		m_gameFinishUI = AddGameObject<GameFinishUI>();

		m_per = AddGameObject<GameFinishUI>();
		TimeManager::CreateInstance();
		TimeManager::GetInstance()->GetTimer().Reset(4);

		for (int i = 0; i < 10; i++) {
			PointManager::GetInstance()->AddPoint(team::TeamType::Blue);
		}
		PointManager::GetInstance()->AddPoint(team::TeamType::Red);

		auto name = uiBuilder.GetUIObject(L"Timer");
		Debug::GetInstance()->Log(name != nullptr);
	}

	void WatanabeStage::OnUpdate() {
		//auto delta = App::GetApp()->GetElapsedTime();
		//auto utilPtr = m_obj->GetBehavior<UtilBehavior>();
		//utilPtr->RotToHead(Vec3(cosf(m_delta), 0, sinf(m_delta)), 2 * delta);
		//m_delta += delta;
		const auto& inputDevice = App::GetApp()->GetMyInputDevice();
		const auto& keyBoard = inputDevice->GetKeyBoard();
		const auto& pad = inputDevice->GetXInputGamePad();
		if (keyBoard.IsInputDown(KeyCode::Alpha1)) {
			m_gameStartUI->Start();
			Debug::GetInstance()->Log(L"StartUI");
		}
		if (keyBoard.IsInputDown(KeyCode::Alpha2)) {
			m_gameFinishUI->Start();
			Debug::GetInstance()->Log(L"FinishUI");
		}
		if (keyBoard.IsInputDown(KeyCode::Alpha0)) {
			m_gameStartUI->Reset();
			m_gameFinishUI->Reset();
			Debug::GetInstance()->Log(L"ResetUI");
		}
		if (pad.IsInputDown(XInputCode::Start)) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToWatanabeStage");
		}
	}
}