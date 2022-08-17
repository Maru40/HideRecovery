/*!
@file   PlayerAnimator.cpp
@brief  プレイヤーのアニメーションクラス実体
*/

#include "stdafx.h"
#include "PlayerAnimator.h"

#include "VelocityManager.h"

namespace basecross {
	PlayerAnimator::PlayerAnimator(const shared_ptr<GameObject>& owner)
		:Animator(owner)
	{}

	void PlayerAnimator::OnCreate() {
		auto data = LoadAnimationData(L"PlayerAnimation");
		for (auto d : data) {
			RegisterAnimationClip(d);
		}
	}

	void PlayerAnimator::OnLateStart() {
		m_velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);
	}

	void PlayerAnimator::OnUpdate() {
		Animator::OnUpdate();

		auto velocityManager = m_velocityManager.lock();
		if (!velocityManager) {
			return;
		}

		constexpr float TransitionSpeed = 0.1f;	//遷移速度
		auto velocity = velocityManager->GetVelocity();

		//速度が一定以下かつ、Dash状態なら
		if (velocity.length() < TransitionSpeed && IsCurretAnimationState(PlayerAnimationState::State::Dash)) {
			ChangePlayerAnimation(PlayerAnimationState::State::Wait);
		}

		//速度が一定より大きいかつ、Wait状態なら
		if (velocity.length() > TransitionSpeed && IsCurretAnimationState(PlayerAnimationState::State::Wait)) {
			ChangePlayerAnimation(PlayerAnimationState::State::Dash);
		}
	}

	void PlayerAnimator::ChangePlayerAnimation(PlayerAnimationState::State state) {
		ChangeAnimation(PlayerAnimationState::PlayerAnimationState2wstring(state));
	}

	bool PlayerAnimator::IsCurretAnimationState(const PlayerAnimationState::State& state) const {
		auto drawer = GetGameObject()->GetComponent<PNTBoneModelDraw>();
		auto strState = PlayerAnimationState::PlayerAnimationState2wstring(state);

		return strState == drawer->GetCurrentAnimation();
	}
}