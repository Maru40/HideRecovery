
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

namespace basecross {

	PlayerDeader::PlayerDeader(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr)
	{}

	void PlayerDeader::OnLateStart() {
		m_animator = GetGameObject()->GetComponent<PlayerAnimator>(false);
	}

	void PlayerDeader::OnUpdate() {
		ObserveAnimation();
	}

	void PlayerDeader::ObserveAnimation() {
		auto animator = m_animator.lock();
		if (!animator) {
			return;
		}

		//アニメーションが死亡状態でないなら監視しない
		if (!animator->IsCurretAnimationState(PlayerAnimationState::State::Dstart)) {
			return;
		}

		if (animator->IsTargetAnimationEnd()) {	//アニメーションが終了したら
			//リスポーン処理がアタッチされているなら
			if (auto respawner = GetGameObject()->GetComponent<Respawner>(false)) {
				respawner->StartRespawn();
			}

			//死亡完了アニメーションに変更
			animator->ChangePlayerAnimation(PlayerAnimationState::State::Wait);
		}
	}

	void PlayerDeader::StartDead() {
		auto animator = m_animator.lock();
		if (animator) {
			animator->ChangePlayerAnimation(PlayerAnimationState::State::Dstart);
		}

		if (auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false)) {
			velocityManager->ResetAll();
		}
	}

	bool PlayerDeader::IsDead() {
		auto status = GetGameObject()->GetComponent<PlayerStatus>(false);
		if (status) {
			return status->IsDead();
		}

		return false;
	}
}