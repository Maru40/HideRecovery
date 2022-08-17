#pragma once
#include "stdafx.h"

namespace basecross {
	struct Status {
		int hp;
		const int maxHp;
		float skillGauge;

		Status(int maxHp)
			:hp(maxHp), maxHp(maxHp), skillGauge(0)
		{}

		void Reset() {
			hp = maxHp;
			skillGauge = 0;
		}
	};

	class PlayerStatus :public Component {
		Status m_status;
	public:
		PlayerStatus(const shared_ptr<GameObject>& owner);

		void OnCreate()override;
		void OnUpdate()override {}
		void OnDraw()override {}

		void AddDamage(int damage);
		bool IsDead();

		void Respawn();
	};
}
