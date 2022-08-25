#pragma once
#include "stdafx.h"
#include "StageObjectBase.h"

namespace basecross {
	class HPGaugeBoardPoly :public StageObjectBase {
	public:
		HPGaugeBoardPoly(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
