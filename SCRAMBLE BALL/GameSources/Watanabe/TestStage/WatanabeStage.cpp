#include "stdafx.h"

#include "WatanabeStage.h"
#include "../DebugClass/Debug.h"
#include "../Effekseer/EfkEffect.h"
#include "Patch/PlayerInputer.h"
#include "Maruyama/Player/Component/PlayerController.h"
#include "Scene.h"

#include "../UI/UIObjects.h"

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

		m_ui = AddGameObject<ReconnectingInGameUI>();
	}

	void WatanabeStage::OnUpdate() {
		const auto& inputDevice = App::GetApp()->GetMyInputDevice();
		const auto& keyBoard = inputDevice->GetKeyBoard();
		const auto& pad = inputDevice->GetXInputGamePad();
		//if (keyBoard.IsInputDown(KeyCode::Alpha1)) {
		//	m_gameStartUI->Start();
		//	Debug::GetInstance()->Log(L"StartUI");
		//}
		if (pad.IsInputDown(XInputCode::Start)) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToWatanabeStage");
		}

		if (pad.IsInputDown(XInputCode::A)) {
			m_ui->SetState(ReconnectingInGameUI::State::InConnection);
		}
		if (pad.IsInputDown(XInputCode::B)) {
			m_ui->SetState(ReconnectingInGameUI::State::Abort);
		}
		if (pad.IsInputDown(XInputCode::Y)) {
			m_ui->SetState(ReconnectingInGameUI::State::None);
		}
	}
}