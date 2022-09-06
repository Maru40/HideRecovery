
/*!
@file ChargeBullet.cpp
@brief ChargeBulletのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "ChargeBullet.h"

#include "Maruyama/Utility/Component/RangeDestroyManager.h"

#include "Watanabe/Component/PlayerStatus.h"
#include "Watanabe/DebugClass/Debug.h"
#include "Itabashi/OnlineManager.h"

#include "StageMapCSV.h"

namespace basecross {

	ChargeBullet::ChargeBullet(const std::shared_ptr<GameObject>& objPtr)
		:BulletBase(objPtr)
	{}

	void ChargeBullet::OnCreate() {
		constexpr int BulletSpeedIndex = 1;
		constexpr int MaxRangeIndex = 3;
		
		auto bulletSpeedStr = StageMapCSV::GetWstringData(L"ShotParametor", L"CSVDatas\\", L"ShotDatas.csv", BulletSpeedIndex);
		auto maxRangeStr = StageMapCSV::GetWstringData(L"ShotParametor", L"CSVDatas\\", L"ShotDatas.csv", MaxRangeIndex);

		auto bulletSpeed = static_cast<float>(_wtof(bulletSpeedStr.c_str()));
		auto maxRange = static_cast<float>(_wtof(maxRangeStr.c_str()));

		SetMoveSpeed(bulletSpeed);
		SetMaxRange(maxRange);
	}

	void ChargeBullet::OnUpdate() {
		BulletBase::OnUpdate();
	}

	void ChargeBullet::Shot(const std::shared_ptr<GameObject>& owner, const Vec3& direct) {
		SetOwner(owner);
		SetMoveDirect(direct);

		if (Online::OnlineManager::GetLocalPlayer().getIsMasterClient())
		{
			//最大距離を設定
			auto rangeDestroy = GetGameObject()->AddComponent<RangeDestoryManager>(GetMaxRange());
		}
	}

	void ChargeBullet::OnCollisionEnter(const CollisionPair& pair) {
		auto other = pair.m_Dest.lock()->GetGameObject();
		if (GetOwner() == other) {	//当たった相手が所有者なら処理をしない
			return;
		}

		auto chargeBullet = other->GetComponent<ChargeBullet>(false);

		if (!Online::OnlineManager::GetLocalPlayer().getIsMasterClient() || chargeBullet)
		{
			return;
		}

		auto player = other->GetComponent<PlayerStatus>(false);
		//ダメージを与える。
		if (player)
		{
			player->AddDamage(DamageData(1, GetOwner()));
		}

		//自分自身を削除
		GetStage()->RemoveGameObject<GameObject>(GetGameObject());
	}

	void ChargeBullet::OnDestroy()
	{
		for (auto& destroyEventFunc : m_destroyEventFuncs)
		{
			destroyEventFunc(GetGameObject());
		}
	}
}