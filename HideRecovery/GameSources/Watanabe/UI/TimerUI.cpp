#include "stdafx.h"
#include "TimerUI.h"
#include "../Manager/TimeManager.h"
#include "../Utility/DataExtracter.h"

namespace basecross {
	TimerUI::TimerUI(const shared_ptr<Stage>& stage)
		:StageObjectBase(stage, L"TimerUI")
	{}

	TimerUI::TimerUI(const shared_ptr<Stage>& stage, const wstring& line)
		: StageObjectBase(stage, L"TimerUI")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		DataExtracter::TransformDataExtraction(tokens, m_transformData);
	}

	void TimerUI::OnCreate() {
		m_numbers = GetStage()->AddGameObject<Numbers>(3);
		m_numbers->SetNumber(300);
		auto numbersTrans = m_numbers->GetComponent<Transform>();
		numbersTrans->SetPosition(m_transformData.Position);
		numbersTrans->SetParent(GetThis<TimerUI>());
	}
	void TimerUI::OnUpdate() {
		float time = TimeManager::GetInstance()->GetTimer().GetLeftTime();
		m_numbers->SetNumber((int)floorf(time));
	}
}