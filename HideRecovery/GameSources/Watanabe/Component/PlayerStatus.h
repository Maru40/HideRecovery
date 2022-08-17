#pragma once
#include "stdafx.h"
#include "I_Damaged.h"

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
	public:
		using DamageFuncType = std::function<void(const std::shared_ptr<PlayerStatus>&, const DamageData&)>;
	private:
		Status m_status;

		std::vector<DamageFuncType> m_damagedFuncs;

	public:
		PlayerStatus(const shared_ptr<GameObject>& owner);

		void OnCreate()override;
		void OnUpdate()override {}
		void OnDraw()override {}

		void AddDamage(const DamageData& damage);
		bool IsDead();
		void Respawn();

		void AddFuncAddDamage(const DamageFuncType& damagedFunc);
	};
}
