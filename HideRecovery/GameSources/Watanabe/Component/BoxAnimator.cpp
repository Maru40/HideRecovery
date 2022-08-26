#include "stdafx.h"
#include "BoxAnimator.h"

namespace basecross {
	BoxAnimator::BoxAnimator(const shared_ptr<GameObject>& owner)
		:Animator(owner)
	{}

	void BoxAnimator::OnCreate() {
		auto data = LoadAnimationData(L"BallAnimation");
		for (auto d : data) {
			RegisterAnimationClip(d);
		}
	}

	void BoxAnimator::ChangeBoxAnimation(BoxAnimationState::State state) {
		ChangeAnimation(BoxAnimationState::BoxAnimationState2wstring(state));
	}
}