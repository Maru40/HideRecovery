#include "stdafx.h"

#include "MaruTestStage.h"

#include "Patch/PlayerInputer.h"
#include "Scene.h"

#include "Watanabe/DebugClass/Debug.h"
#include "Watanabe/UI/UIObjects.h"
#include "Watanabe/Component/Zooming.h"
#include "Watanabe/Camera/TitleCamera.h"

#include "Maruyama/Utility/Utility.h"

#include "Maruyama/Player/Component/OwnArea.h"

#include "Itabashi/ObjectMover.h"
#include "Maruyama/Utility/Component/RotationController.h"
#include "Patch/PlayerInputer.h"
#include "Maruyama/Utility/Utility.h"
#include "Maruyama/Player/Object/HeroPlayerObject.h"
#include "Maruyama/Player/Object/VillainPlayerObject.h"
#include "Patch/CameraHelper.h"
#include "Itabashi/OnlineTransformSynchronization.h"
#include "Patch/SpringArmComponent.h"
#include "Patch/CameraHelper.h"
#include "Maruyama/Camera/Component/LookAtCameraManager.h"

#include "Patch/PlayerMover.h"
#include "Maruyama/Player/Component/PlayerDeader.h"
#include "Maruyama/Player/Object/TestPlayerObject.h"

#include "Maruyama/Utility/Object/GameManagerObject.h"

#include "Maruyama/Enemy/Behavior/Tester/Tester_Behavior.h"

#include "Maruyama/Enemy/Object/AIPlayerObject.h"

namespace basecross {

	void MaruTestStage::CreateViewLight() {
		const Vec3 eye(0.0f, 252.4f, -0.001f);
		const Vec3 at(0, 0.0f, 0);
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

	void MaruTestStage::OnCreate() {
		CreateViewLight();
		auto log = AddGameObject<Debug>();
		Debug::GetInstance()->Log(L"MaruTestStage");

		AddGameObject<GameManagerObject>();

		CreateMap(L"StageS2.csv");

		auto cameraObj = Instantiate<CameraObject>();
		
		//// プレイヤーを見るようなカメラを設定
		auto player = Instantiate<TestPlayerObject>(Vec3(0.0f, 1.0f, -7.0f), Quat::Identity());
		auto sp = player->GetArm()->GetComponent<SpringArmComponent>();
		auto& tpsCamera = sp->GetChildObject();
		tpsCamera->AddComponent<VirtualCamera>(10);
		tpsCamera->AddComponent<LookAtCameraManager>(player, LookAtCameraManager::Parametor());	

		//AddGameObject<GameObject>()->AddComponent<maru::Behavior::TesterTree>();

		Vec3 basePos(8.25f, 1.0f, 50.0f);
		Vec3 positions[] = {
			Vec3(+basePos.x, 1.0f, -basePos.z),
			Vec3(0.0f, 1.0f, -basePos.z),
			Vec3(-basePos.x, 1.0f, -basePos.z),

			Vec3(+basePos.x, 1.0f, basePos.z),
			Vec3(0.0f, 1.0f, basePos.z),

			//Vec3(-basePos.x, 1.0f, basePos.z),
		};

		auto index = 0;
		const Col4 blue(0.0f, 0.0f, 1.0f, 0.0f);
		const Col4 red(1.0f, 0.0f, 0.0f, 1.0f);
		for (auto position : positions) {
			auto enemy = Instantiate<Enemy::AIPlayerObject>(position, Quat::Identity());
			auto color = index < 3 ? blue : red;
			auto teamType = index < 3 ? team::TeamType::Blue : team::TeamType::Red;
			enemy->GetComponent<PNTStaticDraw>()->SetDiffuse(color);
			enemy->GetComponent<I_TeamMember>(false)->SetTeam(teamType);
			index++;
		}

		//auto enemy = Instantiate<Enemy::AIPlayerObject>(Vec3(-30.0f, 1.0f, -6.0f), Quat::Identity());
	}

	void MaruTestStage::OnUpdate() {

	}

}