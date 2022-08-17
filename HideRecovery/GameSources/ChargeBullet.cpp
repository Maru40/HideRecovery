
/*!
@file ChargeBullet.cpp
@brief ChargeBullet�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "ChargeBullet.h"

#include "RangeDestroyManager.h"

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

}