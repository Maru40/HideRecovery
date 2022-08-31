#pragma once
#include "stdafx.h"
#include "StageObjectBase.h"
#include "../Component/PlayerStatus.h"
#include "../Shader/PCTGaugeDraw.h"

namespace basecross {
	class HPGaugeBP :public StageObjectBase {
		weak_ptr<PlayerStatus> m_status;
		shared_ptr<MeshResource> m_meshRes;
		weak_ptr<PCTGaugeDraw> m_drawComp;
	public:
		HPGaugeBP(const shared_ptr<Stage>& stage, const shared_ptr<PlayerStatus>& status);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
