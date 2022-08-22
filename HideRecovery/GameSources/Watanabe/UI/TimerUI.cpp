#include "stdafx.h"
#include "TimerUI.h"
#include "../Manager/TimeManager.h"

namespace basecross {
	TimerUI::TimerUI(const shared_ptr<Stage>& stage)
		:GameObject(stage)
	{}

	void TimerUI::OnCreate() {
		m_numbers = GetStage()->AddGameObject<Numbers>(3);
		m_numbers->SetNumber(300);
		m_numbers->GetComponent<Transform>()->SetParent(GetThis<TimerUI>());
	}
	void TimerUI::OnUpdate() {
		float time = TimeManager::GetInstance()->GetTimer().GetLeftTime();
		m_numbers->SetNumber((int)floorf(time));
	}
}