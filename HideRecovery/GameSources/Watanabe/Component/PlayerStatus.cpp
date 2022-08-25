/*!
@file   PlayerStatus.cpp
@brief  プレイヤーステータスクラス実体
*/

#include "stdafx.h"
#include "PlayerStatus.h"

#include "PlayerInputer.h"

#include "PlayerObject.h"
#include "Maruyama/Player/Component/PlayerDeader.h"

namespace basecross {
	PlayerStatus::PlayerStatus(const shared_ptr<GameObject>& owner)
		:Component(owner), m_status(10), m_team(Team(0))
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

	void PlayerStatus::SetTeam(const Team& team) {
		m_team = team;

		if (auto drawer = GetGameObject()->GetComponent<PlayerObject::DrawComp>(false)) {
			switch (team)
			{
			case Team::West:
				drawer->SetMultiMeshResource(L"Player_Mesh_Blue");
				break;

			case Team::East:
				drawer->SetMultiMeshResource(L"Player_Mesh_Red");
				break;
			}
		}
	}
}