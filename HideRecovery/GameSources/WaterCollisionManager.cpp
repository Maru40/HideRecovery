/*!
@file WaterCollisionManager.cpp
@brief ���̏Փ˔������
*/

#include "stdafx.h"
#include "WaterCollisionManager.h"
#include "WaterPressure.h"

#include "Player.h"
#include "I_PushedWater.h"

namespace basecross
{
	void WaterCollisionManager::OnCollisionEnter(const CollisionPair& pair)
	{
		auto collObj = pair.m_Dest.lock()->GetGameObject(); // ����̃I�u�W�F�N�g���擾
		auto player = dynamic_pointer_cast<Player>(collObj);
		if (!player) // �v���C���[����Ȃ�������
		{
			// ����������
			auto water = dynamic_pointer_cast<WaterPressure>(this->GetGameObject());
			if (water) {
				water->Erasure();
			}
		}

		// ���肪���ɉ�����邩�𔻒�
		auto pushedWater = collObj->GetComponent<I_PushedWater>(false);
		if (pushedWater)
		{
			auto obj = this->GetGameObject(); // �������擾

			PushData data; // �����o���̃f�[�^

			data.direction = this->transform->GetForward(); // �O�������ɉ���
			data.power = 10.0f; // ��������
			data.hitPoint = pair.m_CalcHitPoint; // ���������ʒu

			pushedWater->Push(data); // data�����Ƃɑ���̂��Ƃ�����
		}

	}

	void WaterCollisionManager::OnCollisionExcute(const CollisionPair& pair)
	{
		auto collObj = pair.m_Dest.lock()->GetGameObject(); // �Փ˂���������擾
		auto pushedWater = collObj->GetComponent<I_PushedWater>(false); // ���肪���ɉ�����邩���擾
		if (pushedWater) // ������Ȃ�
		{
			auto obj = this->GetGameObject();

			PushData data; // �����o���̃f�[�^

			data.direction = this->transform->GetForward(); // �O�������ɉ���
			data.power = 10.0f; // ��������
			data.hitPoint = pair.m_CalcHitPoint; // ���������ʒu

			pushedWater->Push(data); // ���������
		}
	}
}