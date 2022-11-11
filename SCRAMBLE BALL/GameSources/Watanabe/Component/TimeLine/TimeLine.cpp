#include "stdafx.h"
#include "TimeLine.h"

namespace basecross {
	TimeLine::TimeLine(const shared_ptr<GameObject>& owner)
		:Component(owner)
	{}

	void TimeLine::OnCreate() {
	}
	void TimeLine::OnUpdate() {
	}
}