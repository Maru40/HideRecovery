#include "stdafx.h"
#include "PasscodeUI.h"

namespace basecross {
	PasscodeUI::PasscodeUI(const shared_ptr<Stage>& stage)
		:UIObjectBase(stage, L"PasscodeUI")
	{}

	void PasscodeUI::OnCreate() {
		m_label = GetStage()->AddGameObject<SimpleSprite>(
			SimpleSprite::Type::SpriteData, L"EnterPasscode"
			);
		m_passCodeNumbers = GetStage()->AddGameObject<Numbers>(4);

		m_label->GetRectTransform()->SetPosition(0, 100);
		m_passCodeNumbers->GetRectTransform()->SetPosition(0, -100);

		m_label->SetParent(GetThis<PasscodeUI>());
		m_passCodeNumbers->SetParent(GetThis<PasscodeUI>());
	}

	void PasscodeUI::UpdateCode(int code) {
		m_passCodeNumbers->SetNumber(code);
	}
}