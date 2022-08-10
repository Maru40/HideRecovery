#include "stdafx.h"
#include "Project.h"

#include "WatanabeStage.h"
#include "../DebugClass/Debug.h"
#include "../Effekseer/EfkEffect.h"
#include "../StageObject/Block.h"
#include "../StageObject/RackObject.h"
#include "../Shader/BoneModelDraw.h"
#include "HeroPlayerObject.h"
#include "CameraHelper.h"

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

		//AddGameObject<CameraObject>();
		//Instantiate<HeroPlayerObject>(Vec3(0.0f, 1.0f, 0.0f), Quat::Identity());
		{
			m_obj = AddGameObject<GameObject>();
			auto testDraw = m_obj->AddComponent<BoneModelDraw>();
			testDraw->SetMultiMeshResource(L"Player_Mesh");
			//testDraw->SetMeshResource(L"rack");
		}

		GameObjecttCSVBuilder builder;
		builder.Register<Block>(L"Block");
		builder.Register<RackObject>(L"Rack");
		auto dir = App::GetApp()->GetDataDirWString();
		auto path = dir + L"MapDatas/";
		builder.Build(GetThis<Stage>(), path + L"StageS2.csv");
	}

	void WatanabeStage::OnUpdate() {
		auto delta = App::GetApp()->GetElapsedTime();
		auto utilPtr = m_obj->GetBehavior<UtilBehavior>();
		utilPtr->RotToHead(Vec3(cosf(m_delta), 0, sinf(m_delta)), 2 * delta);
		m_delta += delta;
	}
}