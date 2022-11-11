#include "stdafx.h"
#include "CameraAnimator.h"

namespace basecross {
	CameraAnimator::CameraAnimator(const shared_ptr<GameObject>& owner)
		:Component(owner)
	{}

	void CameraAnimator::KeyFrameListSort() {
		sort(m_keyFrameList.begin(), m_keyFrameList.end());
	}

	void CameraAnimator::OnCreate() {
	}
	void CameraAnimator::OnUpdate() {
	}
}