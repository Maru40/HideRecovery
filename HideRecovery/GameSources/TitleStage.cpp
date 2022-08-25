#include "stdafx.h"
#include "TitleStage.h"
#include "PlayerInputer.h"
#include "Scene.h"

#include "Watanabe/DebugClass/Debug.h"
#include "Watanabe/UI/UIObjects.h"

namespace basecross {
	void TitleStage::CreateViewLight() {
		const Vec3 eye(20.0f, 20.0f, -20.0f);
		const Vec3 at(5.0f);
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

	void TitleStage::OnCreate() {
		CreateViewLight();
		AddGameObject<Debug>();
		Debug::GetInstance()->Log(L"TitleStage");

		CreateMap(L"StageS2.csv");

		UIObjectCSVBuilder uiBuilder;
		uiBuilder.Register<SimpleSprite>(L"SimpleSprite");
		auto dir = App::GetApp()->GetDataDirWString();
		auto path = dir + L"MapDatas/";
		uiBuilder.Build(GetThis<Stage>(), path + L"TitleUILayout.csv");

		// GetComponent��nullptr�ɂȂ邽�߃R�����g�A�E�g
		//auto titleLogo = uiBuilder.GetUIObject<SimpleSprite>(L"TitleLogo");
		//auto drawComp = titleLogo->GetComponent<PCTSpriteDraw>();
		//if (drawComp) {
		//	drawComp->SetDiffuse(Col4(0, 1, 1, 1));
		//}
		//auto pressA = uiBuilder.GetUIObject<SimpleSprite>(L"PressA");
		//drawComp = pressA->GetComponent<PCTSpriteDraw>();
		//if (drawComp) {
		//	drawComp->SetDiffuse(Col4(0, 0, 0, 1));
		//}
	}
	void TitleStage::OnUpdate() {
		// �}�b�`���O��ʂւ̑J�ڏ���
		const auto& inputDevice = App::GetApp()->GetMyInputDevice();
		const auto& pad = inputDevice->GetXInputGamePad();
		if (pad.IsInputDown(XInputCode::A)) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToMatchingStage");
		}
	}
	void TitleStage::OnDestroy() {
	}
}