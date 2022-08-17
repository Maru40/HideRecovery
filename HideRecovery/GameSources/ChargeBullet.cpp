
/*!
@file ChargeBullet.cpp
@brief ChargeBullet�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "ChargeBullet.h"

#include "RangeDestroyManager.h"

#include "Watanabe/Component/PlayerStatus.h"
#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	ChargeBullet::ChargeBullet(const std::shared_ptr<GameObject>& objPtr)
		:BulletBase(objPtr)
	{}

	void ChargeBullet::OnUpdate() {
		BulletBase::OnUpdate();
	}

	void ChargeBullet::Shot(const std::shared_ptr<GameObject>& owner, const Vec3& direct) {
		SetOwner(owner);
		SetMoveDirect(direct);

		//�ő勗����ݒ�
		auto rangeDestroy = GetGameObject()->AddComponent<RangeDestoryManager>(GetMaxRange());
	}

	void ChargeBullet::OnCollisionEnter(const CollisionPair& pair) {
		auto other = pair.m_Dest.lock()->GetGameObject();
		if (GetOwner() == other) {	//�����������肪���L�҂Ȃ珈�������Ȃ�
			return;
		}

		//�_���[�W��^����B
		if (auto player = other->GetComponent<PlayerStatus>(false)) {
			player->AddDamage(DamageData(1));
			//Debug::GetInstance()->Log(L"Damaged");
		}

		//�������g���폜
		GetStage()->RemoveGameObject<GameObject>(GetGameObject());
	}
}