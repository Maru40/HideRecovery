/*!
@file   PlayerAnimator.cpp
@brief  プレイヤーのアニメーションクラス実体
*/

#include "stdafx.h"
#include "PlayerAnimator.h"

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

	void PlayerAnimator::ChangePlayerAnimation(PlayerAnimationState::State state) {
		ChangeAnimation(PlayerAnimationState::PlayerAnimationState2wstring(state));
	}

	bool PlayerAnimator::IsCurretAnimationState(const PlayerAnimationState::State& state) const {
		auto drawer = GetGameObject()->GetComponent<PNTBoneModelDraw>();
		auto strState = PlayerAnimationState::PlayerAnimationState2wstring(state);

		return strState == drawer->GetCurrentAnimation();
	}
}