/*!
@file InvolvedObjects.cpp
@brief �������܂��I�u�W�F�N�g�Ȃǎ���
*/

#include "stdafx.h"

#include "StageObjectBase.h"
#include "InvolvedObjects.h"
#include "InvolvedManager.h"

#include "Player.h"
#include "Pusher.h"

#include "GameTimer.h"
#include "TimerDestroy.h"
#include "VelocityManager.h"

#include "CollisionObject.h"

namespace basecross
{
	void InvolvedObjects::OnCreate()
	{
		// �`��p�R���|�[�l���g�̐ݒ�
		auto draw = AddComponent<BcPNTStaticModelDraw>();
		draw->SetMeshResource(L"tree_dead2"); // �����ڂ�ݒ�

		//���b�V���̒����pMat
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(0.2f, 1.0f, 1.0f),
			Vec3(0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		draw->SetMeshToTransformMatrix(spanMat); // ���b�V���̐ݒ�

		// �����ʒu�̐ݒ�
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_StartPos);


		// �Փ˔���̐ݒ�
		auto collComp = AddComponent<CollisionObb>();
		collComp->SetMakedSize(1.25f);

		// �R���|�[�l���g�̒ǉ�
		AddComponent<Gravity>();
		AddComponent<InvolvedManager>(GetThis<InvolvedObjects>());
		AddComponent<Pusher>();
		AddComponent<VelocityManager>();

		SetAlphaActive(true); // �����x��K�p����
	}

	void InvolvedObjects::OnUpdate()
	{
		transform->SetScale(Vec3(1.0f / 0.2f, 1.0f, 1.0f));
	}

	void InvolvedObjects::OnDestroy()
	{
		if (m_ColObj)
		{
			// ���������
			GetStage()->RemoveGameObject<GameObject>(m_ColObj.GetShard());
		}
	}

	void InvolvedObjects::Damaged(int damage)
	{
		m_Endurance -= damage; // �_���[�W���̗͂����炷
		Breaking(); // �j�󂷂�
	}

	void InvolvedObjects::Breaking()
	{
		if (m_Endurance <= 0) // �̗͂�0�ȉ��Ȃ�
		{
			// 1�b��Ɏ��g����������
			AddComponent<TimerDestroy>(1.0f);
		}
	}
}