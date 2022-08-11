#include "stdafx.h"
#include "Animator.h"

namespace basecross {
	Animator::Animator(const shared_ptr<GameObject>& owner)
		:Component(owner)
	{}

	bool Animator::LoadMotionData(wstring data) {
	}
}