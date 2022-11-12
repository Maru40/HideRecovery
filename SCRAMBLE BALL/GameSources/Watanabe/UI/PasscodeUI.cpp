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
		auto ptrThis = GetThis<GameObject>();
		std::shared_ptr<RectTransform> rectTransform;

		auto label = GetStage()->AddGameObject<SimpleSprite>(SimpleSprite::Type::SpriteData, L"EnterPasscode");
		label->SetParent(ptrThis);
		rectTransform = label->GetRectTransform();
		rectTransform->SetPosition(0, 100);

		auto passcodeNumbers = GetStage()->AddGameObject<Numbers>(4);
		passcodeNumbers->SetParent(GetThis<PasscodeUI>());
		passcodeNumbers->GetRectTransform()->SetPosition(0, -100);

		Vec2 triangleScale = Vec2(0.35f, 0.35f);

		auto upTriangle = GetStage()->AddGameObject<SimpleSprite>(SimpleSprite::Type::SpriteData, L"Triangle");
		upTriangle->SetParent(passcodeNumbers);
		rectTransform = upTriangle->GetRectTransform();
		rectTransform->SetPosition(0, 120);
		rectTransform->SetRotation(XM_PI);
		rectTransform->SetScale(triangleScale);

		auto downTriangle = GetStage()->AddGameObject<SimpleSprite>(SimpleSprite::Type::SpriteData, L"Triangle");
		downTriangle->SetParent(passcodeNumbers);
		rectTransform = downTriangle->GetRectTransform();
		rectTransform->SetPosition(0, -120);
		rectTransform->SetScale(triangleScale);

		auto passwordTextUI = AddComponent<UI::PasswordTextUI>();

		passwordTextUI->SetNumbers(passcodeNumbers);
		passwordTextUI->SetDecisionSEKey(L"DecisionSE");
		passwordTextUI->SetUpTriangle(upTriangle);
		passwordTextUI->SetDownTriangle(downTriangle);

		AddComponent<UICancelEventer>();
	}
}