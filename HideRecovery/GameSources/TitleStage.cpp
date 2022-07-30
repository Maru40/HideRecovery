#include "stdafx.h"
#include "Project.h"
#include "TitleStage.h"
#include "TitleBoard.h"

#include "SingletonComponent.h"
#include "SoundManager.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	���U���g�X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	void TitleStage::CreateViewLight() {
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

	void TitleStage::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();

			auto& app = App::GetApp();
			auto path = app->GetDataDirWString();
			// �e�N�X�`���̓ǂݍ���
			auto texPath = path + L"Textures/";
			// �e�N�X�`���̓o�^
			auto tpath = app->GetDataDirWString() + L"Textures/";
			app->RegisterTexture(L"Result", tpath + L"Result.png"); // Gauge�e�N�X�`��
			app->RegisterTexture(L"ResultStageClear", tpath + L"ResultStageClear.png"); // StageComplete�e�N�X�`��
			app->RegisterTexture(L"TitleBackGround", tpath + L"TitleBackGround.png"); // �w�i
			app->RegisterTexture(L"GameTitle", tpath + L"GameTitle.png"); // �^�C�g��
			app->RegisterTexture(L"GameStart", tpath + L"GameStart.png"); // �^�C�g��
			app->RegisterTexture(L"Exit", tpath + L"Exit.png"); // �^�C�g��
		}
		catch (...) {
			throw;
		}
		//SoundManager�̎���
		AddGameObject<GameObject>()->AddComponent<SoundManager>();
		const float Volume = 0.15f;
		SoundManager::GetInstance(GetThis<Stage>())->PlayBGM(SoundManager::ID::TitleBGM, Volume);

		// Title�w�i�̒ǉ�
		AddGameObject<TitleBoard>();
		// �Q�[���I�u�W�F�N�g�̒ǉ�

		m_CursorUi = AddGameObject<Cursor>();

		// Ui�̒ǉ�
		std::vector<CursorUIData> titleDatas = {
			CursorUIData(UiData(L"PauseBack", Vec3(0.0f, -165.0f, 0.0f), Vec3(2828.0f, 2481.0f, 0.0f), Vec2(0.16f, 0.06f))),
			CursorUIData(UiData(L"GameTitle", Vec3(0.0f,  200.0f, 0.0f), Vec3(1036.0f,  106.0f, 0.0f), Vec2(1.0f,  1.0f))),
			CursorUIData(UiData(L"GameStart", Vec3(0.0f, -140.0f, 0.0f), Vec3(360.0f,   52.0f, 0.0f), Vec2(1.0f,  1.0f)),[&]() {
				m_sceneflag = true;
				SoundManager::GetInstance(GetThis<Stage>())->PlayOneShot(SoundManager::ID::EnterSE, 0.3f);
				m_FU->FadeIn(); }, true),
			//CursorUIData(UiData(L"Option"	, Vec3(0.0f, -200.0f, 0.0f), Vec3( 256.0f,   64.0f, 0.0f), Vec2(1.0f,  1.0f))),
			CursorUIData(UiData(L"Exit",	  Vec3(0.0f, -200.0f, 0.0f), Vec3(127.0f,   49.0f, 0.0f), Vec2(1.0f,  1.0f)),[]() {}, true)
		};

		for (auto data : titleDatas)
		{
			m_CursorUi->AddUiTexture(data); //UI�̔z�u
		}
		m_CursorUi->SetCursorPos(0);
		m_CursorUi->SetAllDrawLayer(8); //UI�̃��C���[�̐ݒ�
		m_CursorUi->SetDrawLayer(10);

		// �t�F�[�h�C���y�уt�F�[�h�A�E�g�̒ǉ�
		m_FU = AddGameObject<FadeUI>();
	}

	void TitleStage::OnUpdate()
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

		//�R�}���h�̎��s
		if (!m_sceneflag && pad.wPressedButtons & XINPUT_GAMEPAD_A && m_FU->GetColw() <= 0.0f)
		{
			m_CursorUi->Select();
		}

		if (m_FU->GetColw() >= 1.0f)
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(),
				App::GetApp()->GetScene<Scene>(), L"ToStageSelect");
			//SEStop
			SoundManager::GetInstance()->Stop(SoundManager::ID::TitleBGM);
		}

		//�J�[�\���̈ړ�
		if (pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_UP)
		{
			m_CursorUi->MoveCursor(-1);
			const float Volume = 0.15f;
			SoundManager::GetInstance(GetThis<Stage>())->PlayOneShot(SoundManager::ID::CursorSE, Volume);
		}
		if (pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			m_CursorUi->MoveCursor(+1);
			const float Volume = 0.15f;
			SoundManager::GetInstance(GetThis<Stage>())->PlayOneShot(SoundManager::ID::CursorSE, Volume);
		}

	}
}
//end basecross