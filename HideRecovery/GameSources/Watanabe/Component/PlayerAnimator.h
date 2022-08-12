#pragma once
#include "stdafx.h"
#include "Animator.h"
#include "PlayerAnimationState.h"

namespace basecross {
	class PlayerAnimator :public Animator {
	public:
		PlayerAnimator(const shared_ptr<GameObject>& owner);

		void OnCreate()override;

		void ChangePlayerAnimation(PlayerAnimationState state);
	};
}
