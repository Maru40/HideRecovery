#include "stdafx.h"
#include "PostProcess.h"

namespace basecross {
	PostProcess::PostProcess(const shared_ptr<GameObject>& owner)
		:Component(owner)
	{}

	void PostProcess::OnPostDraw() {
	}
}