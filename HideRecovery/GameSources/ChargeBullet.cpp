
/*!
@file ChargeBullet.cpp
@brief ChargeBulletのクラス実体
担当：丸山裕喜
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

		//最大距離を設定
		auto rangeDestroy = GetGameObject()->AddComponent<RangeDestoryManager>(GetMaxRange());
	}

	void ChargeBullet::OnCollisionEnter(const CollisionPair& pair) {
		auto other = pair.m_Dest.lock()->GetGameObject();
		if (GetOwner() == other) {	//当たった相手が所有者なら処理をしない
			return;
		}

		//ダメージを与える。
		if (auto player = other->GetComponent<PlayerStatus>(false)) {
			player->AddDamage(DamageData(1));
			//Debug::GetInstance()->Log(L"Damaged");
		}

		//自分自身を削除
		GetStage()->RemoveGameObject<GameObject>(GetGameObject());
	}
}