/*!
@file CrushScore.cpp
@brief �ׂ������̐����������
*/

#include "stdafx.h"
#include "Scene.h"
#include "CrushScore.h"

#include "GameStage.h"
#include "PolygonTexture.h"

namespace basecross
{
	void CrushScore::OnUpdate()
	{
		// UI���ړ�������
		MoveUi();
	}

	void CrushScore::DeployUi(const wstring& texture)
	{
		auto stage = GetStage(); // ���݂̃X�e�[�W���擾
		// UI��z�u
		auto data = UiData(texture, m_StartPos, Vec3(256.0 * 0.7f, 64.0f * 0.7, 0.0f), Vec2(1.0f, 1.0f));
		m_ScoreUi.push_back(stage->AddGameObject<PolygonTexture>(data));
	}

	void CrushScore::MoveUi()
	{
		for (auto& ui : m_ScoreUi)
		{
			auto delta = App::GetApp()->GetElapsedTime(); // �f���^���Ԃ��擾

			auto uiTrans = ui->GetComponent<Transform>(); // UI�̃g�����X�t�H�[�����擾
			auto pos = uiTrans->GetPosition(); // UI�̃|�W�V�������擾

			// �|�W�V�������X�V
			pos += m_MovePower * delta;
			uiTrans->SetPosition(pos);

			FadeOut(ui, pos, delta);
		}
	}

	void CrushScore::FadeOut(const std::shared_ptr<GameObject>& UI, const Vec3& position, float delta)
	{
		if (position.y >= 50) // Y������50�ȏ�Ȃ�
		{
			// �t�F�[�h�A�E�g������
			auto color = UI->GetComponent<PCTSpriteDraw>()->GetDiffuse();
			color.w -= 3.0f * delta;
			UI->GetComponent<PCTSpriteDraw>()->SetDiffuse(color);

			if (color.w <= 0) // �t�F�[�h�A�E�g���I�������
			{
				// �`��ƍX�V�̏������~�߂�
				UI->SetUpdateActive(false);
				UI->SetDrawActive(false);
			}
		}
	}
}