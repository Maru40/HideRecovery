#include "stdafx.h"
#include "TestEffectStage.h"
#include "../DebugClass/Debug.h"
#include "../Effekseer/EfkEffect.h"
#include "../Camera/TestCamera.h"
#include "../StageObject/CameraTarget.h"
#include "Watanabe/StageObject/StageObjects.h"

#include "../UI/UIObjects.h"

namespace basecross {
	void TestEffectStage::CreateViewLight() {
		const Vec3 eye(5.0f, 5.0f, -5.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<GameCamera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetTarget(AddGameObject<CameraTarget>(PtrCamera));
		//PtrCamera->SetNear(5);
		//PtrCamera->SetFar(10);
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}

	void TestEffectStage::OnCreate() {
		CreateViewLight();
		AddGameObject<Debug>();
		Debug::GetInstance()->Log(L"TestEffectStage");
		AddGameObject<EfkInterface>();

		GameObjecttCSVBuilder builder;
		builder.Register<Block>(L"Block");
		builder.Register<RackObject>(L"Rack");
		builder.Register<HideItemObject>(L"HideItem");
		builder.Register<GoalObject>(L"Goal");
		builder.Register<OwnAreaObject>(L"OwnAreaObject");
		builder.Register<Container>(L"Container");
		auto dir = App::GetApp()->GetDataDirWString();
		auto path = dir + L"MapDatas/";
		builder.Build(GetThis<Stage>(), path + L"StageS2.csv");

		AddGameObject<SkyBox>(Vec3(500));

		EfkEffectResource::RegisterEffectResource(L"Explosion", dir + L"Effects/" + L"Explosion.efk");

		auto effectObject = AddGameObject<GameObject>();
		m_efkComp = effectObject->AddComponent<EfkComponent>();
		m_efkComp->SetEffectResource(L"Explosion", TransformData(Vec3(0, 0, 80.25), Vec3(7, 10, 7)), true);
		m_efkComp->SetEffectResource(L"HasBall", TransformData(Vec3(0, 0.5f, 0), Vec3(0.5f)));

		auto ui = AddGameObject<DirectionWithHasBallUI>();
		ui->SetTarget(effectObject);

		m_ui = AddGameObject<InProcessUI>();
	}

	void TestEffectStage::OnUpdate() {
		if (m_controller.GetPressedButtons(ControllerButton::A)) {
			m_efkComp->Play(L"Explosion");
		}

		if (m_controller.GetPressedButtons(ControllerButton::B)) {
			m_efkComp->Stop(L"HasBall");
			m_efkComp->PlayLoop(L"HasBall");
		}

		if (m_controller.GetPressedButtons(ControllerButton::X)) {
			dynamic_pointer_cast<InProcessUI>(m_ui)->SetLabel(
				InProcessUI::LabelType::Matching
			);
			m_ui->SetActive(true);
			Debug::GetInstance()->Log(L"Active");
		}
		if (m_controller.GetPressedButtons(ControllerButton::Y)) {
			m_ui->SetActive(false);
		}
	}
}