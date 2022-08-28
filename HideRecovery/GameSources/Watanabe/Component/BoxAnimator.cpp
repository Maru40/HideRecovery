#include "stdafx.h"
#include "BoxAnimator.h"

namespace basecross {
	BoxAnimator::BoxAnimator(const shared_ptr<GameObject>& owner)
		:Animator(owner)
	{}

	void BoxAnimator::OnCreate() {
		auto data = LoadAnimationData(L"BoxAnimation");
		for (auto d : data) {
			RegisterAnimationClip(d);
		}
	}

	void BoxAnimator::ChangeBoxAnimation(BoxAnimationState::State state) {
		ChangeAnimation(BoxAnimationState::BoxAnimationState2wstring(state));
	}

	bool BoxAnimator::IsCurrentAnimator(const BoxAnimationState::State& state) const {
		auto drawer = GetGameObject()->GetComponent<PNTBoneModelDraw>();
		auto strState = BoxAnimationState::BoxAnimationState2wstring(state);

		return strState == drawer->GetCurrentAnimation();
	}
}