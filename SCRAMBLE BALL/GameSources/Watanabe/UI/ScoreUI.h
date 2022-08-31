#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"
#include "Maruyama/Interface/I_TeamMember.h"

namespace basecross {
	class ScoreUI :public UIObjectBase {
		// 表記としてのプレイヤー番号
		int m_playerNumber;
		// 処理内のプレイヤー番号
		int m_gamePlayerNumber;
		team::TeamType m_teamType;
	public:
		ScoreUI(const shared_ptr<Stage>& stage, int playerNumber, team::TeamType teamType);
		ScoreUI(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;
	};
}
