#include "stdafx.h"
#include "ClipBase.h"

namespace basecross {
	namespace timeline {
		ClipBase::ClipBase(const shared_ptr<GameObject>& owner)
			:Component(owner)
		{}

		void ClipBase::OnCreate() {
		}

		void ClipBase::Update(float delta) {
		}
		void ClipBase::AddKeyFrame() {
		}
	}
}