#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"

namespace basecross {
	class ScoreUI :public UIObjectBase {
		int m_playerNumber;
	public:
		ScoreUI(const shared_ptr<Stage>& stage, int playerNumber);
		ScoreUI(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;
	};
}
