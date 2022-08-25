#pragma once
#include "stdafx.h"
#include "StageObjectBase.h"
#include "../Component/PlayerStatus.h"
#include "../Shader/PCTGaugeDraw.h"

namespace basecross {
	class HPGaugeBoardPoly :public StageObjectBase {
		weak_ptr<PlayerStatus> m_status;
		weak_ptr<PCTGaugeDraw> m_drawComp;
	public:
		HPGaugeBoardPoly(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
