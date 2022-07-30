/*!
@file GameStart.cpp
@brief �J�n���o�Ȃǎ���
*/

#include "stdafx.h"
#include "GameStart.h"

#include "GameStage.h"
#include "PolygonTexture.h"
#include "TimeManager.h"
#include "Freeze.h"
#include "ChangeCameraManager.h"

#include "GameManager.h"

namespace basecross
{
	void GameStart::OnCreate()
	{
		// �X�e�[�W�ɔ|����ǉ�
		auto stage = GetStage();
		m_Go = stage->AddGameObject<PolygonTexture>(UiData(L"Go", Vec3(0.0f, 0.0f, 0.0f), Vec3(256.0f, 128.0f, 0.0f), Vec2(0.0f, 0.0f)));
		m_Ready = stage->AddGameObject<PolygonTexture>(UiData(L"Ready", Vec3(0.0f, -500.0f, 0.0f), Vec3(512.0f, 128.0f, 0.0f), Vec2(1.0f, 1.0f)));

		m_ReadyTrans = m_Ready->GetComponent<Transform>();
		m_GoTrans = m_Go->GetComponent<Transform>();

		// �^�C���}�l�[�W���R���|�[�l���g��ǉ�
		m_TimerComp = AddComponent<TimeManager>();
	}

	void GameStart::OnUpdate()
	{
		auto readyPos = m_ReadyTrans->GetPosition();
		if (readyPos.y <= m_ReadyMaxPosY) //Ready�̃|�W�V�������I�_����Ȃ��Ȃ�Ready�𓮂���
		{
			Ready();
		}
		else // Ready���I�_�ɒ�������Go��傫������
		{
			Go();
		}
	}

	void GameStart::Ready()
	{
		ChangeCameraManager::GetInstance()->SetUpdateActive(false); //�^�[�Q�b�g�J�������g���Ȃ�����

		auto delta = App::GetApp()->GetElapsedTime(); // �f���^���Ԃ��擾
		auto readyPos = m_ReadyTrans->GetPosition(); // �|�W�V�������擾

		// �|�W�V�����̍X�V
		auto newPos = readyPos;
		newPos.y += m_ReadyMoveSpeed * delta;
		m_ReadyTrans->SetPosition(newPos);
	}

	void GameStart::Go()
	{
		auto delta = App::GetApp()->GetElapsedTime(); //�f���^���Ԃ��擾
		auto goScale = m_GoTrans->GetScale(); // �X�P�[�����擾

		if (m_GoMaxScale.x >= goScale.x)
		{
			//�ő�܂ő傫���Ȃ��Ă��Ȃ��Ȃ�傫������
			auto newScale = goScale + Vec3(m_GoGrowthSpeed) * delta;
			m_GoTrans->SetScale(newScale);
		}
		else
		{
			auto timer = dynamic_pointer_cast<TimeManager>(m_TimerComp);
			auto data = TimeData(L"End", 2.0f, [&]() {End(); });
			timer->AddTimer(data); // End�֐����Ăяo���^�C�}�[���`

			auto time = timer->GetTimeRate(L"End"); //End�̃^�C�����[�g���擾

			// �A���t�@�l��������
			auto draw = m_Go->GetComponent<PCTSpriteDraw>();
			auto color = draw->GetDiffuse();
			color.w = time;
			draw->SetDiffuse(color);
		}
	}

	void GameStart::End()
	{
		// �X�e�[�W�ɒǉ�����Ă���I�u�W�F�N�g�S�Ă̍X�V�������ĊJ����
		auto objes = GetStage()->GetGameObjectVec(); // �X�e�[�W�ɔz�u����Ă���I�u�W�F�N�g���擾
		for (auto obj : objes)
		{
			auto isFreeze = obj->GetComponent<Freeze>(false);
			if (isFreeze) // Freeze�R���|�[�l���g�������Ă�����
			{
				obj->SetUpdateActive(true); // �X�V���ĊJ
			}
		}

		ChangeCameraManager::GetInstance()->SetUpdateActive(true); //�^�[�Q�b�g�J�������g����悤�ɂ���

		// Ready������
		m_Ready->SetDrawActive(false);
		m_Ready->SetUpdateActive(false);

		// Go������
		m_Go->SetDrawActive(false);
		m_Go->SetUpdateActive(false);

		// ���g������
		this->SetDrawActive(false);
		this->SetUpdateActive(false);

		// �Q�[�����v���C���ɑJ��
		GameManager::GetInstance()->ChangeState(GameManager::State::Game);
	}
}