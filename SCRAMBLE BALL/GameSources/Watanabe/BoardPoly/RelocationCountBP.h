#pragma once
#include "stdafx.h"
#include "Maruyama/StageObject/StageObjectBase.h"

namespace basecross {
	class RelocationCountBP :public StageObjectBase {
		weak_ptr<PCTStaticDraw> m_drawComp;
	public:
		RelocationCountBP(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
