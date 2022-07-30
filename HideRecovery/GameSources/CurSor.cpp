/*!
@file Corsor.cpp
@brief �J�[�\���Ȃǎ���
*/

#include "stdafx.h"
#include "Cursor.h"
#include "PolygonTexture.h"

namespace basecross
{
	void Cursor::OnCreate()
	{
		//�|���̒�`
		Col4 color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		std::vector<VertexPositionColorTexture> vertices = {
			{Vec3(-16.0f, +16.0f, 0.0f), color, Vec2(0.0f, 0.0f)},
			{Vec3(+16.0f, +16.0f, 0.0f), color, Vec2(1.0f, 0.0f)},
			{Vec3(-16.0f, -16.0f, 0.0f), color, Vec2(0.0f, 1.0f)},
			{Vec3(+16.0f, -16.0f, 0.0f), color, Vec2(1.0f, 1.0f)}
		};
		std::vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3
		};

		auto drawComp = AddComponent<PCTSpriteDraw>(vertices, indices); //�`��p�R���|�[�l���g�̐ݒ�
		drawComp->SetTextureResource(L"Cursor"); //�e�N�X�`���̐ݒ�

		SetDrawActive(true); // �`���Ԃ�ݒ�
		SetAlphaActive(true); // �����x��K�p
		SetDrawLayer(0); // �`�惌�C���[��ݒ�

	}

	void Cursor::AddUiTexture(const CursorUIData& data)
	{
		m_UiObj.push_back(data); // �Ǘ�����UI�f�[�^��ǉ�
		m_cursorObj.push_back(GetStage()->AddGameObject<PolygonTexture>(data.uiData)); // �X�e�[�W��UI��ǉ�
	}

	void Cursor::ClearUITexture()
	{
		m_cursorObj.clear();
		m_UiObj.clear();
	}

	void Cursor::Select()
	{
		if (m_UiObj[m_CursorPos].func.operator bool()) // �֐������s�\�Ȃ�
		{
			m_UiObj[m_CursorPos].func(); // ���s
		}
	}

	void Cursor::MoveCursor(int pos)
	{
		m_CursorPos += pos; //�|�W�V�������X�V

		PositionCorrection(pos); // �|�W�V������␳

		PositionCorrection(-pos); // �z��O�ɏo�Ă���\��������̂ŕ␳

		// ��������g�����X�t�H�[���̕ύX

		auto cursorTrans = GetComponent<Transform>(); // �g�����X�t�H�[�����擾�@
		auto cursorScale = cursorTrans->GetScale(); // �X�P�[�����擾

		auto uiData = m_UiObj[m_CursorPos].uiData; // UI�̃f�[�^���擾

		auto newPos = uiData.position; // UI�̈ʒu���擾
		newPos.x -= uiData.vertexPos.x / 2 * uiData.scale.x + cursorScale.x * m_PositionOffset; // UI�̍����ɔz�u�����悤�ɏC��
		cursorTrans->SetPosition(newPos); // �|�W�V�������X�V
	}

	void Cursor::PositionCorrection(int pos)
	{
		// �I��s�Ȃ��΂�
		while (!m_UiObj[m_CursorPos].isSelectable)
		{
			m_CursorPos += pos;
		}

		// �z�u����Ă���UI�̐����擾
		int size = (int)m_UiObj.size();

		// �|�W�V�������z��͈̔͊O�Ȃ�͈͓��ɓ����
		if (m_CursorPos >= size)
		{
			m_CursorPos = size - 1;
		}
		if (m_CursorPos < 0)
		{
			m_CursorPos = 0;
		}
	}
}