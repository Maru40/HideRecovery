
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
#include "Itabashi/OnlineManager.h"

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

		if (!Online::OnlineManager::GetLocalPlayer().getIsMasterClient())
		{
			return;
		}

		auto player = other->GetComponent<PlayerStatus>(false);
		//ダメージを与える。
		if (player)
		{
			player->AddDamage(DamageData(1, GetOwner()));
		}

		for (auto& destroyEventFunc : m_destroyEventFuncs)
		{
			destroyEventFunc(GetGameObject());
		}

		//自分自身を削除
		GetStage()->RemoveGameObject<GameObject>(GetGameObject());
	}
}