
/*!
@file PlayerDeader.cpp
@brief PlayerDeaderクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "PlayerDeader.h"

#include "Maruyama/Player/Component/Respawner.h"

#include "Watanabe/Component/PlayerAnimator.h"
#include "Watanabe/Component/PlayerStatus.h"

#include "VelocityManager.h"
#include "UseWepon.h"

namespace basecross {

	PlayerDeader::PlayerDeader(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr),
		m_updateFunction(nullptr)
	{}

	void PlayerDeader::OnLateStart() {
		m_animator = GetGameObject()->GetComponent<PlayerAnimator>(false);

		auto animator = m_animator.lock();
		if (animator) {
			PlayerAnimationState::State states[] = {
				PlayerAnimationState::State::Dead,
				PlayerAnimationState::State::GSDead,
			};

			auto update = [&, animator]() {
				if (m_updateFunction) {
					m_updateFunction();
				}
				return false;
			};

			for (auto& state : states) {
				animator->AddAnimationEvent(state, nullptr, update, nullptr);
			}
		}
	}

	void PlayerDeader::OnUpdate() {

	}

	void PlayerDeader::ObserveAnimation() {
		auto animator = m_animator.lock();
		if (!animator) {
			return;
		}

		//アニメーションが死亡状態でないなら監視しない
		if (!animator->IsCurretAnimationState(PlayerAnimationState::State::Dead) && 
			!animator->IsCurretAnimationState(PlayerAnimationState::State::GSDead))
		{
			return;
		}

		if (animator->IsTargetAnimationEnd()) {	//アニメーションが終了したら
			//リスポーン処理がアタッチされているなら
			if (auto respawner = GetGameObject()->GetComponent<Respawner>(false)) {
				respawner->StartRespawn();
			}

			m_updateFunction = nullptr;	//更新をやめる。
		}
	}

	void PlayerDeader::StartDead() {
		auto useWeapon = GetGameObject()->GetComponent<UseWepon>(false);
		auto animator = m_animator.lock();

		if (animator && useWeapon) {
			//IsAimなら
			if (useWeapon->IsAim()) {
				animator->ChangePlayerAnimation(PlayerAnimationState::State::GSDead);
			}
			else {
				animator->ChangePlayerAnimation(PlayerAnimationState::State::Dead);
			}
		}

		if (auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false)) {
			velocityManager->ResetAll();
		}

		m_updateFunction = [&]() { ObserveAnimation(); };	//更新処理設定
	}

	bool PlayerDeader::IsDead() {
		auto status = GetGameObject()->GetComponent<PlayerStatus>(false);
		if (status) {
			return status->IsDead();
		}

		return false;
	}
}