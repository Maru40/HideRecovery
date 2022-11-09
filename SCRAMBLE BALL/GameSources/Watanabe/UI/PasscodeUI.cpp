#include "stdafx.h"
#include "PasscodeUI.h"
#include "Itabashi/PasswordTextUI.h"
#include "Itabashi/UICancelEventer.h"

namespace basecross {
	PasscodeUI::PasscodeUI(const shared_ptr<Stage>& stage)
		:UIObjectBase(stage, L"PasscodeUI")
	{}

	void PasscodeUI::OnCreate()
	{
		auto label = GetStage()->AddGameObject<SimpleSprite>(
			SimpleSprite::Type::SpriteData, L"EnterPasscode"
			);

		auto passcodeNumbers = GetStage()->AddGameObject<Numbers>(4);

		label->GetRectTransform()->SetPosition(0, 100);
		passcodeNumbers->GetRectTransform()->SetPosition(0, -100);

		label->SetParent(GetThis<PasscodeUI>());
		passcodeNumbers->SetParent(GetThis<PasscodeUI>());

		auto passwordTextUI = AddComponent<UI::PasswordTextUI>();

		passwordTextUI->SetNumbers(passcodeNumbers);
		passwordTextUI->SetDecisionSEKey(L"DecisionSE");

		AddComponent<UICancelEventer>();
	}
}