#pragma once
#include "stdafx.h"

namespace basecross {
	class PostProcess :public Component {
	public:
		PostProcess(const shared_ptr<GameObject>& owner);

		void OnPostDraw();
	};
}
