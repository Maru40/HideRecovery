#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"
#include "../Shader/PCTGaugeSprite.h"
#include "Itabashi/MatchStageTransitioner.h"

namespace basecross {
	class HoldButtonUI :public UIObjectBase {
		weak_ptr<PCTGaugeSprite> m_drawComp;
		weak_ptr<MatchStageTransitioner> m_transitioner;
	public:
		HoldButtonUI(const shared_ptr<Stage>& stage);
		HoldButtonUI(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;
		void OnUpdate()override;

		void SetTransitioner(const shared_ptr<MatchStageTransitioner>& transitioner) {
			m_transitioner = transitioner;
		}
	};
}
