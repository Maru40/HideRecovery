
/*!
@file ChargeBullet.cpp
@brief ChargeBullet�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "ChargeBullet.h"

#include "RangeDestroyManager.h"

#include "I_Damaged.h"

namespace basecross {

	ChargeBullet::ChargeBullet(const std::shared_ptr<GameObject>& objPtr)
		:BulletBase(objPtr)
	{}

	void ChargeBullet::OnUpdate() {
		BulletBase::OnUpdate();
	}

	void ChargeBullet::Shot(const Vec3& direct) {
		SetMoveDirect(direct);

		//�ő勗����ݒ�
		auto rangeDestroy = GetGameObject()->AddComponent<RangeDestoryManager>(GetMaxRange());
	}

	void ChargeBullet::OnCollisionEnter(const CollisionPair& pair) {
		auto other = pair.m_Dest.lock()->GetGameObject();

		//�_���[�W��^����B
		if (auto damaged = other->GetComponent<I_Damaged>(false)) {
			damaged->Damaged(DamageData(1.0f));
		}

		//�������g���폜
		GetStage()->RemoveGameObject<GameObject>(GetGameObject());
	}
}