#pragma once
#include "stdafx.h"
#include "StageObjectBase.h"

namespace basecross {
	class RelocationCountBP :public StageObjectBase {
	public:
		RelocationCountBP(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
