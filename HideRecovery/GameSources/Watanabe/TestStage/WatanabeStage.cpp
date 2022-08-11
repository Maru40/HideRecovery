﻿#include "stdafx.h"
#include "Project.h"

#include "WatanabeStage.h"
#include "../DebugClass/Debug.h"
#include "../Effekseer/EfkEffect.h"
#include "../StageObject/Block.h"
#include "../StageObject/RackObject.h"
#include "../Shader/BoneModelDraw.h"
#include "HeroPlayerObject.h"
#include "VillainPlayerObject.h"
#include "CameraHelper.h"
#include "Itabashi/PlayerOnlineController.h"
#include "Itabashi/OnlineTransformSynchronization.h"
#include "SpringArmComponent.h"
#include "CameraHelper.h"
#include "LookAtCameraManager.h"
#include "../Component/TestComponent.h"
#include "Itabashi/ObjectMover.h"
#include "RotationController.h"
#include "PlayerInputer.h"
#include "MaruUtility.h"
#include "../Utility/CSVLoad.h"
#include "../Component/PlayerAnimator.h"

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
		{
			wstring mediaDir;
			mediaDir = App::GetApp()->GetDataDirWString();
			wstring dir = mediaDir + L"Models/";
			CSVLoad::GetInstance()->RegisterFile(L"PlayerAnimation", dir + L"Player/PlayerAnimation.csv");
		}
		auto testObj = AddGameObject<GameObject>();
		auto efkComp = testObj->AddComponent<EfkComponent>();
		efkComp->SetEffectResource(L"TestEffect");
		//efkComp->PlayLoop(L"TestEffect");

		AddGameObject<CameraObject>();
		// ヴィランプレイヤーを生成
		auto player = Instantiate<VillainPlayerObject>(Vec3(0.0f, 1.0f, 0.0f), Quat::Identity());
		// テストのためオンライン系のコンポーネントを削除
		player->RemoveComponent<Online::PlayerOnlineController>();
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

		player->AddComponent<PlayerAnimator>();

		GameObjecttCSVBuilder builder;
		builder.Register<Block>(L"Block");
		builder.Register<RackObject>(L"Rack");
		auto dir = App::GetApp()->GetDataDirWString();
		auto path = dir + L"MapDatas/";
		builder.Build(GetThis<Stage>(), path + L"StageS2.csv");
	}

	void WatanabeStage::OnUpdate() {
		//auto delta = App::GetApp()->GetElapsedTime();
		//auto utilPtr = m_obj->GetBehavior<UtilBehavior>();
		//utilPtr->RotToHead(Vec3(cosf(m_delta), 0, sinf(m_delta)), 2 * delta);
		//m_delta += delta;
	}
}