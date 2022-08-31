#pragma once
#include "stdafx.h"
#include "StageObjectBase.h"

namespace basecross {
	class SkyBox :public StageObjectBase {
	public:
		SkyBox(const shared_ptr<Stage>& stage, const Vec3& scale);

		void OnCreate()override;
	};
}
