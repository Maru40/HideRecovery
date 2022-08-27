#include "stdafx.h"
#include "ScoreUI.h"

namespace basecross {
	ScoreUI::ScoreUI(const shared_ptr<Stage>& stage, int playerNumber)
		:UIObjectBase(stage, L"ScoreUI")
	{}

	ScoreUI::ScoreUI(const shared_ptr<Stage>& stage, const wstring& line)
		: UIObjectBase(stage, L"ScoreUI")
	{}

	void ScoreUI::OnCreate() {
	}
}