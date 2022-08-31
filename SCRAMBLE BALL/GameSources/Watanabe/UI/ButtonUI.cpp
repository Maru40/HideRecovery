#include "stdafx.h"
#include "ButtonUI.h"
#include "../Manager/SpriteDataManager.h"
#include "../Utility/DataExtracter.h"

namespace basecross {
	ButtonUI::ButtonUI(const shared_ptr<Stage>& stage)
		:UIObjectBase(stage, L"ButtonUI"), m_buttonStr(L"A")
	{}
	ButtonUI::ButtonUI(const shared_ptr<Stage>& stage, const wstring& line)
		: UIObjectBase(stage, L"ButtonUI")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		size_t nextIndex = DataExtracter::RectTransformDataExtraction(tokens, m_rectTransformData);
		m_buttonStr = tokens[nextIndex];
	}

	void ButtonUI::OnCreate() {
		const auto& stage = GetStage();
		m_buttonBack = stage->AddGameObject<SimpleSprite>(SimpleSprite::Type::SpriteData, L"ButtonBack");
		m_buttonText = stage->AddGameObject<SimpleSprite>(SimpleSprite::Type::SpriteData, m_buttonStr + L"Button");

		m_buttonBack->SetParent(GetThis<ButtonUI>());
		m_buttonText->SetParent(GetThis<ButtonUI>());
	}

	void ButtonUI::SetActive(bool flg) {
		m_buttonBack->SetActive(flg);
		m_buttonText->SetActive(flg);
	}

	void ButtonUI::ChangeButton(const wstring& buttonString) {
		m_buttonText->ChangeSprite(SimpleSprite::Type::SpriteData, buttonString + L"Button");
	}
}