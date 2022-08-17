
/*!
@file ChargeBullet.cpp
@brief ChargeBulletのクラス実体
担当：丸山裕喜
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

		//最大距離を設定
		auto rangeDestroy = GetGameObject()->AddComponent<RangeDestoryManager>(GetMaxRange());
	}

	void ChargeBullet::OnCollisionEnter(const CollisionPair& pair) {
		auto other = pair.m_Dest.lock()->GetGameObject();

		//ダメージを与える。
		if (auto damaged = other->GetComponent<I_Damaged>(false)) {
			damaged->Damaged(DamageData(1.0f));
		}

		//自分自身を削除
		GetStage()->RemoveGameObject<GameObject>(GetGameObject());
	}
}