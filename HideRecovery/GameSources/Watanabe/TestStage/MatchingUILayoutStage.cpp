#include "stdafx.h"
#include "MatchingUILayoutStage.h"
#include "../UI/UIObjects.h"
#include "../DebugClass/Debug.h"

namespace basecross {
	void MatchingUILayoutStage::CreateViewLight() {
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
	void MatchingUILayoutStage::OnCreate() {
		CreateViewLight();
		AddGameObject<Debug>();
		Debug::GetInstance()->Log(L"MatchingUILayoutStage");
	}
	void MatchingUILayoutStage::OnUpdate() {
	}
}