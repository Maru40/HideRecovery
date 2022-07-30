/*!
@file Pause.cpp
@brief �|�[�Y��ʂȂǎ���
*/

#include "stdafx.h"
#include "Scene.h"
#include "Pause.h"

#include "PolygonTexture.h"
#include "Cursor.h"

#include "GameStage.h"
#include "SoundManager.h"

namespace basecross
{
	void Pause::OnCreate()
	{
		// �|���̒�`
		Col4 color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		std::vector<VertexPositionColorTexture> vertices = {
			{Vec3(-500.0f, +500.0f, 0.0f), color, Vec2(0.0f, 0.0f)},
			{Vec3(+500.0f, +500.0f, 0.0f), color, Vec2(1.0f, 0.0f)},
			{Vec3(-500.0f, -500.0f, 0.0f), color, Vec2(0.0f, 1.0f)},
			{Vec3(+500.0f, -500.0f, 0.0f), color, Vec2(1.0f, 1.0f)}
		};
		std::vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3
		};

		DeployUi(); //Ui�̔z�u

		// �`��̐ݒ�
		auto drawComp = AddComponent<PCTSpriteDraw>(vertices, indices);
		drawComp->SetTextureResource(L"PauseBack"); //�e�N�X�`���̐ݒ�

		// �g�����X�t�H�[���̐ݒ�
		auto transComp = GetComponent<Transform>();
		Vec3 pos(0.0f, 0.0f, 0.0f);
		transComp->SetScale(0.6f, 0.6f, 1.0f);
		transComp->SetPosition(pos);

		SetAlphaActive(true); // �����x��K�p
		SetDrawActive(false); // �`������Ȃ�

		SetDrawLayer(0); // �`�惌�C���[�̐ݒ�
	}

	void Pause::OnUpdate()
	{
		//�R���g���[���[�̎擾
		auto& app = App::GetApp();
		auto device = app->GetInputDevice();
		const auto& pad = device.GetControlerVec()[0];

		if (pad.wPressedButtons & XINPUT_GAMEPAD_START && !m_IsGameOver) // �X�^�[�g�{�^���������ꂽ��
		{
			SetPause(); // �|�[�Y������
		}

		// �J�[�\���̈ړ�
		MoveCursor();
	}

	void Pause::DeployUi()
	{
		auto stage = GetStage(); // �X�e�[�W���擾

		// �|�[�Y��ʗp�̃J�[�\���̔z�u
		m_Cursor = stage->AddGameObject<Cursor>();
		for (auto uiData : pauseDatas)
		{
			m_Cursor->AddUiTexture(uiData); //UI�̔z�u
		}
		m_Cursor->SetCursorPos(0);
		m_Cursor->SetAllDrawActive(false); //�`�������
		m_Cursor->SetAllDrawLayer(2); //�`�悷�郌�C���[�̐ݒ�
	}

	void Pause::SetPause()
	{
		auto setValue = IsDrawActive(); //�`�悳��Ă��邩���擾
		SetDrawActive(!setValue); //�`���Ԃ𔽓]
		m_Cursor->SetAllDrawActive(!setValue); //�J�[�\���̕`���Ԃ𔽓]
		m_Cursor->SetCursorPos(-1); //�J�[�\���̈ʒu����ԏ��

		Pausing(setValue); //���Ԓ�~������
	}

	void Pause::GameOver()
	{
		m_Cursor->ClearUITexture(); // Pause��ʂ�UI���폜
		// UI�̔z�u
		for (auto uiData : gameOverDatas)
		{
			m_Cursor->AddUiTexture(uiData); //UI�̔z�u
		}
		m_Cursor->SetCursorPos(0);
		m_Cursor->SetAllDrawActive(false); //�`�������
		m_Cursor->SetAllDrawLayer(2); //�`�悷�郌�C���[�̐ݒ�

		auto setValue = IsDrawActive(); //�`�悳��Ă��邩���擾
		SetDrawActive(!setValue); //�`���Ԃ𔽓]
		m_Cursor->SetAllDrawActive(!setValue); //�J�[�\���̕`���Ԃ𔽓]
		Pausing(setValue); //���Ԓ�~������
		m_IsGameOver = true;
	}

	void Pause::Pausing(bool value)
	{
		auto stageObj = GetGameStage()->GetGameObjectVec(); //�X�e�[�W�ɔz�u����Ă���I�u�W�F�N�g�̎擾
		for (auto obj : stageObj)
		{
			auto poseUi = dynamic_pointer_cast<Pause>(obj);
			if (!poseUi) //Pause����Ȃ�������
			{
				obj->SetUpdateActive(value); //�X�V���~�߂�
			}
		}

	}

	void Pause::MoveCursor()
	{
		if (GetDrawActive())
		{
			//�R���g���[���[�̎擾
			auto& app = App::GetApp();
			auto device = app->GetInputDevice();
			const auto& pad = device.GetControlerVec()[0];

			// �J�[�\������Ɉړ�
			if (pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_UP)
			{
				// SE���Đ�
				auto soundManager = SoundManager::GetInstance();
				if (soundManager) {
					soundManager->PlayOneShot(SoundManager::ID::CursorSE, 0.15f);
				}

				// �|�[�Y��
				m_Cursor->MoveCursor(-1);
			}

			// ���Ɉړ�
			if (pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN)
			{
				// SE���Đ�
				auto soundManager = SoundManager::GetInstance();
				if (soundManager) {
					soundManager->PlayOneShot(SoundManager::ID::CursorSE, 0.15f);
				}

				// �|�[�Y��
				m_Cursor->MoveCursor(+1);
			}

			//�R�}���h�̎��s
			if (pad.wPressedButtons & XINPUT_GAMEPAD_A)
			{
				// SE���Đ�
				auto soundManager = SoundManager::GetInstance();
				if (soundManager) {
					soundManager->PlayOneShot(SoundManager::ID::EnterSE, 0.15f);
				}

				// �|�[�Y��
				m_Cursor->Select();
			}
		}
	}
}