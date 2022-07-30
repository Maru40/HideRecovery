#include "stdafx.h"
#include "Project.h"
#include "StageSelect.h"
#include "TitleBoard.h"
#include "StageSelectUI.h"
#include "FadeUI.h"

#include "BuilderVertexPCT.h"
#include "SpriteObject.h"

#include "StageSelectSpriteManager.h"

#include "SingletonComponent.h"
#include "FadeSprite.h"

#include "PlayerInputer.h"

#include "SingletonComponent.h"
#include "SoundManager.h"

#include "DebugObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	���U���g�X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	void StageSelect::CreateViewLight() {
		const Vec3 eye(0.0f, 15.0f, -20.0f);
		const Vec3 at(0.0f);
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



	void StageSelect::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();

			auto& app = App::GetApp();
			auto path = app->GetDataDirWString();
			// �e�N�X�`���̓ǂݍ���
			auto texPath = path + L"Textures/";
			// �e�N�X�`���̓o�^
			auto tpath = app->GetDataDirWString() + L"Textures/";
			app->RegisterTexture(L"TitleBackGround", tpath + L"TitleBackGround.png"); // �w�i

		}
		catch (...) {
			throw;
		}
		// �t�F�[�h�C���y�уt�F�[�h�A�E�g�̒ǉ�
		m_FU = AddGameObject<FadeUI>();

		//SoundManager�̎���
		AddGameObject<GameObject>()->AddComponent<SoundManager>();
		const float Volume = 0.15f;
		SoundManager::GetInstance(GetThis<Stage>())->PlayBGM(SoundManager::ID::SelectBGM, Volume);

		//// Title�w�i�̒ǉ�
		//AddGameObject<TitleBoard>();
		const float Size = 1.0f;
		const Vec3 offset(-30.0f, -85.0f, 0.0f);
		auto backGroudParam = Builder::VertexPCTParametor(Vec3(1280.0f, 800.0f, 0.0f) * Size, Vec2(1280.0f, 800.0f), L"SelectBack_TX");
		AddGameObject<SpriteObject>(backGroudParam);

		//// �Q�[���I�u�W�F�N�g�̒ǉ�
		AddGameObject<StageSelectTitle>();

		//SelectStageManager
		AddGameObject<GameObject>()->AddComponent<StageSelectSpriteManager>();

		AddGameObject<GameObject>()->AddComponent<FadeSpriteManager>();

		//debug------------------------------
		AddGameObject<DebugObject>();
	}

	void StageSelect::OnUpdate()
	{
		// �A�v���̎Q�Ǝ擾
		auto& app = App::GetApp();

		static const auto& inputDevice = app->GetMyInputDevice();
		static const auto& keyBoard = inputDevice->GetKeyBoard();

		// �f�o�C�X�̎擾
		auto device = app->GetInputDevice();
		const auto& pad = device.GetControlerVec()[0];

		// �X�e�B�b�N�̌X�����x�N�g���ɕύX
		Vec3 LeftStick(pad.fThumbLX, 0.0f, pad.fThumbLY);

		//if (PlayerInputer::IsLBDown()) {
		//	FadeSpriteManager::GetInstance()->FadeStart(FadeType::Out);
		//}
		//if (PlayerInputer::IsRBDown()) {
		//	FadeSpriteManager::GetInstance()->FadeStart(FadeType::In);
		//}

		//if (!m_sceneflag && pad.wPressedButtons & XINPUT_GAMEPAD_A && m_FU->GetColw() <= 0.0f)
		//{
		//	m_sceneflag = true;
		//	m_FU->FadeIn();
		//}
		//if (m_FU->GetColw() >= 1.0f)
		//{
		//	PostEvent(0.0f, GetThis<ObjectInterface>(),
		//		App::GetApp()->GetScene<Scene>(), L"ToMaruTestStage");
		//}
	}
}
//end basecross