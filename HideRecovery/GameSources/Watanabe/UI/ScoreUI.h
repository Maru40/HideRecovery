#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"
#include "Maruyama/Interface/I_TeamMember.h"

namespace basecross {
	class ScoreUI :public UIObjectBase {
		int m_playerNumber;
		team::TeamType m_teamType;
	public:
		ScoreUI(const shared_ptr<Stage>& stage, int playerNumber, team::TeamType teamType);
		ScoreUI(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;
	};
}
