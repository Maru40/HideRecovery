#include "stdafx.h"
#include "GoalMessageUI.h"

namespace basecross {
	GoalMessageUI::GoalMessageUI(const shared_ptr<Stage>& stage)
		:MessageUI(stage)
	{}

	void GoalMessageUI::OnCreate() {
		m_sprite = GetStage()->AddGameObject<SimpleSprite>(SimpleSprite::Type::SpriteData, L"Goal");
		m_sprite->SetActive(false);
	}

	void GoalMessageUI::Start() {
		m_sprite->SetActive(true);
	}

	void GoalMessageUI::Reset() {
		m_sprite->SetActive(false);
	}
}