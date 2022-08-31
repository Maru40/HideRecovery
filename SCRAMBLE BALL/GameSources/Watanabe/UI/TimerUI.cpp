#include "stdafx.h"
#include "TimerUI.h"
#include "../Manager/TimeManager.h"
#include "../Utility/DataExtracter.h"

namespace basecross {
	TimerUI::TimerUI(const shared_ptr<Stage>& stage)
		:UIObjectBase(stage, L"TimerUI")
	{}

	TimerUI::TimerUI(const shared_ptr<Stage>& stage, const wstring& line)
		: UIObjectBase(stage, L"TimerUI")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		DataExtracter::RectTransformDataExtraction(tokens, m_rectTransformData);
	}

	void TimerUI::OnCreate() {
		m_numbers = GetStage()->AddGameObject<Numbers>(3);
		m_numbers->SetNumber(300);
		m_numbers->SetParent(GetThis<TimerUI>());
	}
	void TimerUI::OnUpdate() {
		float time = TimeManager::GetInstance()->GetTimer().GetLeftTime();
		m_numbers->SetNumber((int)std::ceilf(time));
	}
}