/*!
@file   PlayerStatus.cpp
@brief  プレイヤーステータスクラス実体
*/

#include "stdafx.h"
#include "PlayerStatus.h"

#include "PlayerInputer.h"

#include "Maruyama/Player/Component/PlayerDeader.h"

namespace basecross {
	PlayerStatus::PlayerStatus(const shared_ptr<GameObject>& owner)
		:Component(owner), m_status(10)
	{}

	void PlayerStatus::OnCreate() {
	}

	void PlayerStatus::OnUpdate() {

	}

	void PlayerStatus::AddDamage(const DamageData& damage) {
		if (IsDead()) {
			return;
		}

		m_status.hp -= damage.value;

		// 念のため0にクランプ
		if (m_status.hp <= 0) {
			if (auto deader = GetGameObject()->GetComponent<PlayerDeader>(false)) {
				deader->StartDead();
			}

			m_status.hp = 0;
		}

		for (auto& damagedFunc : m_damagedFuncs)
		{
			damagedFunc(GetThis<PlayerStatus>(), damage);
		}
	}

	bool PlayerStatus::IsDead() {
		return m_status.hp <= 0;
	}

	void PlayerStatus::Respawn() {
		m_status.Reset();
	}

	void PlayerStatus::AddFuncAddDamage(const DamageFuncType& damagedFunc)
	{
		m_damagedFuncs.push_back(damagedFunc);
	}
}