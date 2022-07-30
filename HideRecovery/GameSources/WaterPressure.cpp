/*!
@file WaterPressure.cpp
@brief �W�F�b�g�|���v�Ȃǎ���
*/

#include "stdafx.h"
#include "GameStage.h"
#include "WaterPressure.h"
#include "WaterCollisionManager.h"

#include "LaserBase.h"
#include "LaserCollider.h"

#include "WaterDraw.h"

namespace basecross
{
	void WaterPressure::OnCreate()
	{
		// �`��p�R���|�[�l���g��ǉ�
		AddComponent<WaterDraw>();

		// �^�O��ǉ�
		AddTag(L"Water");
	}

	void WaterPressure::Move()
	{
		// �f���^���Ԃ��擾
		auto delta = App::GetApp()->GetElapsedTime();

		auto transComp = GetComponent<Transform>(); // �g�����X�t�H�[�����擾
		auto myPos = transComp->GetPosition(); // �|�W�V�������擾

		// �|�W�V�������X�V
		auto newPos = myPos + m_MoveDir * m_MovePow * delta;
		newPos.y = m_StartPos.y;

		transComp->SetPosition(newPos); // �|�W�V�������Z�b�g
		transform->SetForward(m_MoveDir); // ������O���ɉ�]
	}

	void WaterPressure::CapturedCamera()
	{
		std::vector<Vec3> points = { // �i�s�����ɑ΂��Č��ɒ��_��z�u����
			m_MoveDir * 0.0f,
			m_MoveDir * -m_Weight,
			m_MoveDir * (-m_Height + m_Weight),
			m_MoveDir * -m_Height
		};

		int index = 0;

		auto camera = GetStage()->GetView()->GetTargetCamera(); // �J�������擾
		std::vector<VertexPositionColorTexture> vertices;
		auto myPos = GetComponent<Transform>()->GetPosition();

		for (auto& point : points) // ���S�_�̍��E�ɒ��_��z�u����
		{
			auto pointToCamera = camera->GetEye() - (point + myPos); // �|�C���g���u���[���h���W�Ɉړ��������ʒu�v����݂��J�����̕���
			auto cross = pointToCamera.cross(m_MoveDir); // �u�J�����ւ̃x�N�g���v�Ɓu�i�s�����v�Ɂu�����ȃx�N�g���v���Z�o
			cross.normalize(); // �x�N�g���̌��������g�������̂Ő��K������
			vertices[index++].position = point + -cross * m_Weight; // ��
			vertices[index++].position = point + +cross * m_Weight; // �E
		}
		auto drawComp = GetComponent<PCTStaticDraw>();
		drawComp->UpdateVertices(vertices); // �ʒu��ύX�������_��K�p����

	}

	void WaterPressure::Erasure()
	{
		// �`��ƍX�V�����Ȃ�����
		SetDrawActive(false);
		SetUpdateActive(false);
	}
}