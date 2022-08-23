#pragma once
#include "stdafx.h"
#include "../Component/PlayerStatus.h"
#include "../Shader/PCTGaugeSprite.h"

namespace basecross {
	class HPGaugeUI :public GameObject {
		weak_ptr<PlayerStatus> m_status;
		weak_ptr<PCTGaugeSprite> m_drawComp;
	public:
		HPGaugeUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
