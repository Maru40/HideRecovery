#pragma once
#include "stdafx.h"
#include "Animator.h"
#include "BallAnimationState.h"

namespace basecross {
	class BallAnimator :public Animator {
	public:
		BallAnimator(const shared_ptr<GameObject>& owner);

		void OnCreate() override;

		/// <summary>
		/// アニメーションの変更
		/// </summary>
		/// <param name="state">変更するアニメーションのステート</param>
		void ChangeBallAnimation(BallAnimationState::State state);
	};
}
