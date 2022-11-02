/*!
@file   PlayerStatus.cpp
@brief  プレイヤーステータスクラス実体
*/

#include "stdafx.h"
#include "PlayerStatus.h"

#include "Patch/PlayerInputer.h"

#include "Maruyama/Player/Object/PlayerObject.h"
#include "Maruyama/Player/Component/PlayerDeader.h"

#include "Maruyama/Player/Component/ItemBag.h"
#include "Maruyama/Item/HideItem.h"

#include "Watanabe/Manager/ScoreManager.h"
#include "Itabashi/OnlinePlayerSynchronizer.h"

#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"

namespace basecross {
	PlayerStatus::PlayerStatus(const shared_ptr<GameObject>& owner)
		:Component(owner), m_status(10), m_team(team::TeamType(0)),
		m_damageSoundClip(L"PlayerDamageSE", false, 0.75f),
		m_inAreaSoundClip(L"AlertSE_00", false, 0.05f)
	{}

	void PlayerStatus::OnCreate() {
		if (auto shareManager = ShareClassesManager::GetInstance(GetStage())) {
			shareManager->AddShareClass<I_TeamMember>(GetGameObject());
		}
	}

	void PlayerStatus::OnLateStart()
	{
		m_soundEmitter = GetGameObject()->GetComponent<SoundEmitter>(false);

		AddReactiveIsInAreaEvent(true, [&]() { 
			if (auto soundEmitter = m_soundEmitter.lock()) {
				soundEmitter->PlaySoundClip(m_inAreaSoundClip);
			}
		});
	}

	void PlayerStatus::OnUpdate() {
	}

	void PlayerStatus::AddDamage(const DamageData& damage) {
		if (IsDead()) {
			return;
		}

		//同チームなら攻撃しない
		auto attackerMember = damage.attacker->GetComponent<I_TeamMember>(false);
		if (attackerMember && attackerMember->GetTeam() == GetTeam()) {
			return;
		}

		m_status.hp -= damage.value;

		// 念のため0にクランプ
		if (m_status.hp <= 0) {
			//キルカウントを加算
			auto playerController = damage.attacker->GetComponent<OnlinePlayerSynchronizer>(false);
			if (playerController) {
				ScoreManager::GetInstance()->AddKillCount(playerController->GetGamePlayerNumber());
			}

			if (auto deader = GetGameObject()->GetComponent<PlayerDeader>(false)) {
				deader->StartDead();
			}

			m_status.hp = 0;
		}

		auto soundEmitter = m_soundEmitter.lock();
		soundEmitter->PlaySoundClip(m_damageSoundClip);

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

	void PlayerStatus::SetTeam(const team::TeamType& team) {
		m_team = team;

		if (auto drawer = GetGameObject()->GetComponent<PlayerObject::DrawComp>(false)) {
			switch (team)
			{
			case team::TeamType::Red:
				drawer->SetTextureResource(L"RedHeroTx");
				drawer->SetModelTextureEnabled(false);
				break;

			case team::TeamType::Blue:
				drawer->SetTextureResource(L"BlueHeroTx");
				drawer->SetModelTextureEnabled(false);
				break;
			}
		}
	}
}