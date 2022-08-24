#include "stdafx.h"
#include "BallAnimator.h"

namespace basecross {
	BallAnimator::BallAnimator(const shared_ptr<GameObject>& owner)
		:Animator(owner)
	{}

	void BallAnimator::OnCreate() {
		auto data = LoadAnimationData(L"BallAnimation");
		for (auto d : data) {
			RegisterAnimationClip(d);
		}
	}

	void BallAnimator::ChangePlayerAnimation(BallAnimatorState::State state) {
		ChangeAnimation(BallAnimatorState::BallAnimatorState2wstring(state));
	}
}