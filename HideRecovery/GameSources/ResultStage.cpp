#include "stdafx.h"
#include "Project.h"
#include "ResultStage.h"
#include "Result.h"
#include "StageComplete.h"
#include "Score.h"
#include "ResultTime.h"
#include "ResultBoard.h"
#include "SoundManager.h"
#include "Player.h"
#include "GameManagerObject.h"
#include "MainStage.h"

#include "Animator_Player.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	���U���g�X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	void ResultStage::CreateViewLight() {
		const Vec3 eye(0.0f, 0.0f, -20.0f);
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



	void ResultStage::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();

			AddGameObject<GameManagerObject>();

			//AddGameObject<GameObject>()->AddComponent<SoundManager>();
			//SoundManager::GetInstance(GetThis<Stage>())->PlayOneShot();

			//SEStop
			//SoundManager::GetInstance()->Stop(SoundManager::ID::GameBGM);
			//SoundManager::GetInstance()->Stop(SoundManager::ID::ClearSE);
			auto& app = App::GetApp();
			auto path = app->GetDataDirWString();
			// �e�N�X�`���̓ǂݍ���
			auto texPath = path + L"Textures/";
			// �e�N�X�`���̓o�^
			auto tpath = app->GetDataDirWString() + L"Textures/";
			app->RegisterTexture(L"ResultBackGround", tpath + L"ResultBackGround.png"); // �w�i

			app->RegisterTexture(L"Result", tpath + L"Result.png"); // Gauge�e�N�X�`��
			app->RegisterTexture(L"ResultStageClear", tpath + L"ResultStageClear.png"); // StageComplete�e�N�X�`��

						// SE�̍Đ�
			//AddGameObject<GameObject>()->AddComponent<SoundManager>();
			SoundManager::GetInstance(GetThis<Stage>())->PlayBGM(SoundManager::ID::ClearSE, 0.15f, 0);
		}
		catch (...) {
			throw;
		}

		// �t�F�[�h�C���y�уt�F�[�h�A�E�g�̒ǉ�
		m_FU = AddGameObject<FadeUI>();
		// Result�w�i�̒ǉ�
		AddGameObject<ResultBoard>();

		// �Q�[���I�u�W�F�N�g�̒ǉ�
		auto result = AddGameObject<Result>();
		auto stageComplete = AddGameObject<StageComplete>();
		auto time = AddGameObject<ResultTime>();
		auto timeM = AddGameObject<ResultTimeMinute>();
		auto score = AddGameObject<Score>();
		auto player = AddGameObject<Player>();
		auto playertrans = player->GetComponent<Transform>();
		auto resultrank = AddGameObject<ResultRank>();
		auto resultNext = AddGameObject<ResultNext>();
		auto resultRetry = AddGameObject<ResultRetry>();
		auto abotton = AddGameObject<ABotton>();
		auto bbotton = AddGameObject<BBotton>();
		playertrans->SetPosition(+30.0f, -3.0f, 0.0f);
		//playertrans->SetRotation(-0.4f, 3.0f, 0.1f);
		playertrans->SetForward(Vec3(0.0f, 0.0f, -1.0f));
		playertrans->SetScale(5.0f, 5.0f, 5.0f);

		auto animator = player->GetComponent<PlayerAnimator>(false);
		if (animator) {
			animator->ChangeState(PlayerAnimator::StateEnum::WinMove);
		}
	}

	void ResultStage::OnUpdate()
	{
		// �A�v���̎Q�Ǝ擾
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		static const auto& inputDevice = app->GetMyInputDevice();
		static const auto& keyBoard = inputDevice->GetKeyBoard();

		// �f�o�C�X�̎擾
		auto device = app->GetInputDevice();
		const auto& pad = device.GetControlerVec()[0];

		// �X�e�B�b�N�̌X�����x�N�g���ɕύX
		Vec3 LeftStick(pad.fThumbLX, 0.0f, pad.fThumbLY);

		// �P�ʎ��Ԃ̎擾
		float deltaTime = app->GetElapsedTime();


		if (!m_sceneflag && pad.wPressedButtons & XINPUT_GAMEPAD_A && m_FU->GetColw() <= 0.0f)
		{
			m_sceneflag = true;
			SoundManager::GetInstance(GetThis<Stage>())->PlayOneShot(SoundManager::ID::EnterSE, 0.3f);
			m_sceneMode = 1;
			m_FU->FadeIn();
		}
		if (m_FU->GetColw() >= 1.0f && m_sceneMode == 1)
		{
			auto soundManager = SoundManager::GetInstance();
			if (soundManager) {
				soundManager->Stop(SoundManager::ID::ClearSE);
			}

			int a = scene->GetStageName();
			MainStage::SetLoadMapName(m_StageName[scene->GetStageName() + 1]); // Stage�ԍ����Ă�ł��Ď��̃X�e�[�W��ǂ߂�悤�ɂ���
			PostEvent(0.0f, GetThis<ObjectInterface>(),
				App::GetApp()->GetScene<Scene>(), L"ToMainStage");
		}


		if (!m_sceneflag && pad.wPressedButtons & XINPUT_GAMEPAD_B && m_FU->GetColw() <= 0.0f)
		{
			SoundManager::GetInstance(GetThis<Stage>())->PlayOneShot(SoundManager::ID::EnterSE, 0.3f);
			m_sceneflag = true;
			m_sceneMode = 2;
			m_FU->FadeIn();
		}
		if (m_FU->GetColw() >= 1.0f && m_sceneMode == 2)
		{
			auto soundManager = SoundManager::GetInstance();
			if (soundManager) {
				soundManager->Stop(SoundManager::ID::ClearSE);
			}

			int a = scene->GetStageName();
			MainStage::SetLoadMapName(m_StageName[scene->GetStageName()]);
			PostEvent(0.0f, GetThis<ObjectInterface>(),
				App::GetApp()->GetScene<Scene>(), L"ToMainStage");
		}
	}
}
//end basecross