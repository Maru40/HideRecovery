#pragma once
#include "stdafx.h"
#include "Animator.h"

namespace basecross {
	enum class PlayerAnimation {
		Wait,
		Walk_R,
		Walk_L,
		Dash,
		PutItem_Floor,
		PutItem_HideObject,
	};

	class PlayerAnimator :public Animator {
	public:
		PlayerAnimator(const shared_ptr<GameObject>& owner);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
