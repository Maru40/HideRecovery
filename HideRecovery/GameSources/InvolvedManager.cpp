/*!
@file InvolvedManager.cpp
@brief �������܂��I�u�W�F�N�g�̏Փ˔���
*/

#include "stdafx.h"
#include "InvolvedManager.h"
#include "InvolvedObjects.h"

#include "Player.h"
#include "Crusher.h"

namespace basecross
{
	void InvolvedManager::OnCollisionEnter(const CollisionPair& pair)
	{
		auto collObj = pair.m_Dest.lock()->GetGameObject(); // �Փˑ�����擾
		auto player = dynamic_pointer_cast<Player>(collObj); // ���肪�v���C���[������
		if (player)
		{
			ObjectRotation(pair.m_DestCalcHitCenter); // ��]
		}

		auto crusher = collObj->GetComponent<Crusher>(false); // �j��҂��𒲂ׂ�
		if (crusher)
		{
			HitDamage(pair.m_CalcHitPoint); // �_���[�W����
		}
	}

	void InvolvedManager::HitDamage(const Vec3& point)
	{
		auto myPos = transform->GetPosition(); // �����̈ʒu
		auto scale = transform->GetScale(); //�傫��

		auto range = m_InvolObj->GetRangeFactor(); // �_���[�W���󂯂�͈͂��擾
		auto hitScope = Vec2(scale.x * range.x, scale.z * range.y); // ��_���[�W�͈�
		hitScope *= 0.1;

		//�@���������ʒu���͈͂̒��Ȃ�
		if ((point.x <= myPos.x + hitScope.x && point.x >= myPos.x - hitScope.x) ||
			(point.z <= myPos.z + hitScope.y && point.z >= myPos.z - hitScope.y))
		{
			m_InvolObj->Damaged(1); // �_���[�W��^����
		}
	}

	void InvolvedManager::ObjectRotation(const Vec3& hitCenter)
	{
		auto position = transform->GetPosition(); //���݂̈ʒu
		auto right = transform->GetRight(); //�E����
		auto forward = transform->GetForward(); //�O����
		auto hitDir = hitCenter - position; //�q�b�g��������

		auto hitRight = dot(hitDir.normalize(), right.normalize()); //���������ʒu���E�����ɑ΂���+��-��
		auto hitForward = dot(hitDir.normalize(), forward.normalize()); //���������ʒu���O�����ɑ΂���+��-��

		auto scope = m_InvolObj->GetRotationScope();

		//���S���瓖�������ʒu�܂ł̋��������ȉ��Ȃ�
		if (hitDir.length() <= scope)
		{
			auto delta = App::GetApp()->GetElapsedTime();
			transform->Rotate(Vec3::Up() * m_RotationPow * hitRight * hitForward * delta); //��]������
		}
	}
}