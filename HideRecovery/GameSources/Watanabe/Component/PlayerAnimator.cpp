#include "stdafx.h"
#include "PlayerAnimator.h"

namespace basecross {
	PlayerAnimator::PlayerAnimator(const shared_ptr<GameObject>& owner)
		:Animator(owner)
	{}

	void PlayerAnimator::OnCreate() {
	}
	void PlayerAnimator::OnUpdate() {
	}
}