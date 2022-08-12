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

	void PlayerAnimator::ChangePlayerAnimation(PlayerAnimationState state) {
		switch (state)
		{
		case PlayerAnimationState::Wait:
			ChangeAnimation(L"Wait");
			break;
		case PlayerAnimationState::Walk_R:
			ChangeAnimation(L"Walk_R");
			break;
		case PlayerAnimationState::Walk_L:
			ChangeAnimation(L"Walk_L");
			break;
		case PlayerAnimationState::Dash:
			ChangeAnimation(L"Dash");
			break;
		case PlayerAnimationState::PutItem_Floor:
			ChangeAnimation(L"PutItem_Floor");
			break;
		case PlayerAnimationState::PutItem_HideObject:
			ChangeAnimation(L"PutItem_HideObject");
			break;
		default:
			break;
		}
	}
}