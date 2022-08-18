#include "stdafx.h"
#include "OnlineMatchStage.h"
#include "PlayerInputer.h"
#include "Scene.h"
#include "OnlineMatching.h"

namespace basecross
{
	void OnlineMatchStage::OnCreate()
	{
		const Vec3 eye(0.0f, 5.0f, -5.0f);
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

		auto gameObject = AddGameObject<GameObject>();

		auto onlineMatching = gameObject->AddComponent<Online::OnlineMatching>();
	}

	void OnlineMatchStage::OnUpdate()
	{
		if (PlayerInputer::IsDecision())
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToMainStage");
			return;
		}
		if (PlayerInputer::IsCancel())
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
		}
	}
}