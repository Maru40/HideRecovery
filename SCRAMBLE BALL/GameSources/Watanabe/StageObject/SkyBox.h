#pragma once
#include "stdafx.h"
#include "Maruyama/StageObject/StageObjectBase.h"

namespace basecross {
	class SkyBox :public StageObjectBase {
	public:
		SkyBox(const shared_ptr<Stage>& stage, const Vec3& scale);

		void OnCreate()override;
	};
}
