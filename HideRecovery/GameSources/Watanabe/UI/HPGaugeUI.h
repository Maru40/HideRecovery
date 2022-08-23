#pragma once
#include "stdafx.h"
#include "../Component/PlayerStatus.h"
#include "../Shader/PCTGaugeSprite.h"
#include "UIObjectBase.h"

namespace basecross {
	class HPGaugeUI :public UIObjectBase {
		weak_ptr<PlayerStatus> m_status;
		weak_ptr<PCTGaugeSprite> m_drawComp;
	public:
		HPGaugeUI(const shared_ptr<Stage>& stage);
		HPGaugeUI(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
