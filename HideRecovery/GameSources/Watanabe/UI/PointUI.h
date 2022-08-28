#pragma once
#include "stdafx.h"
#include "Numbers.h"
#include "Maruyama/Interface/I_TeamMember.h"
#include "UIObjectBase.h"

namespace basecross {
	class PointUI :public UIObjectBase {
		shared_ptr<Numbers> m_numbers;
		team::TeamType m_team;
	public:
		PointUI(const shared_ptr<Stage>& stage);
		PointUI(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
