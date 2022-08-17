/*!
@file   PlayerStatus.cpp
@brief  プレイヤーステータスクラス実体
*/

#include "stdafx.h"
#include "PlayerStatus.h"

namespace basecross {
	PlayerStatus::PlayerStatus(const shared_ptr<GameObject>& owner)
		:Component(owner), m_status(10)
	{}

	void PlayerStatus::OnCreate() {
	}

	void PlayerStatus::AddDamage(DamageData damage) {
		m_status.hp -= damage.value;

		// 念のため0にクランプ
		if (m_status.hp <= 0)
			m_status.hp = 0;
	}

	bool PlayerStatus::IsDead() {
		return m_status.hp <= 0;
	}

	void PlayerStatus::Respawn() {
		m_status.Reset();
	}
}