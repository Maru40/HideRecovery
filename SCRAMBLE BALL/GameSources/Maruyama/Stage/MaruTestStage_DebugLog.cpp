#include "stdafx.h"

#include "MaruTestStage_DebugLog.h"

#include "Watanabe/DebugClass/Debug.h"

#include "Maruyama/Thread/ThreadPool.h"

#include "Patch/PlayerInputer.h"

#include "Maruyama/Enemy/ImpactMap/Cell.h"
#include "Maruyama/Enemy/ImpactMap/FieldCellMap.h"

namespace basecross {

	void MaruTestStage_DebugLog::CreateViewLight() {
		const Vec3 eye(0.0f, 30.0f, -0.000001f);
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

	void MaruTestStage_DebugLog::OnCreate() {
		CreateViewLight();
		auto log = AddGameObject<Debug>();
		Debug::GetInstance()->Log(L"MaruTestStage_DebugLog");

		AddGameObject<Tester::TesterThreadObject>();

		//Cellのデバッグ表示
		//auto cellParam = maru::Cell::Parametor();
		//auto cell = std::make_shared<maru::Cell>(cellParam);
		//cell->OnDebugDraw(GetThis<Stage>());

		AddGameObject<GameObject>()->AddComponent<maru::FieldCellMap>();
	}

	void MaruTestStage_DebugLog::OnUpdate() {
		if (PlayerInputer::GetInstance()->IsBDown()) {
			
		}
	}

}