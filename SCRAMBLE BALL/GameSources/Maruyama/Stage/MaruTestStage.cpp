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
#include "Itabashi/PlayerOnlineController.h"
#include "Itabashi/OnlineTransformSynchronization.h"
#include "Patch/SpringArmComponent.h"
#include "Patch/CameraHelper.h"
#include "Maruyama/Camera/Component/LookAtCameraManager.h"

#include "Patch/PlayerMover.h"
#include "Maruyama/Player/Component/PlayerDeader.h"
#include "Maruyama/Player/Object/TestPlayerObject.h"

#include "Maruyama/Utility/Object/GameManagerObject.h"

#include "Maruyama/Enemy/Behavior/Tester/Tester_Behavior.h"

namespace basecross {

	void MaruTestStage::CreateViewLight() {
		const Vec3 eye(0.0f, 252.4f, -0.001f);
		const Vec3 at(0, 0.0f, 0);
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
	}

	void MaruTestStage::OnCreate() {
		CreateViewLight();
		auto log = AddGameObject<Debug>();
		Debug::GetInstance()->Log(L"MapShotStage");

		CreateMap(L"StageS2.csv");

		auto cameraObj = Instantiate<CameraObject>();

		AddGameObject<GameManagerObject>();
		
		//// �v���C���[������悤�ȃJ������ݒ�
		auto player = Instantiate<TestPlayerObject>(Vec3(0.0f, 1.0f, -5.0f), Quat::Identity());
		auto sp = player->GetArm()->GetComponent<SpringArmComponent>();
		auto& tpsCamera = sp->GetChildObject();
		tpsCamera->AddComponent<VirtualCamera>(10);
		tpsCamera->AddComponent<LookAtCameraManager>(player, LookAtCameraManager::Parametor());	

		AddGameObject<GameObject>()->AddComponent<maru::Behavior::TesterTree>();
	}

	void MaruTestStage::OnUpdate() {

	}

}