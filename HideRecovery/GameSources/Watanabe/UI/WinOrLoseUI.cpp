#include "stdafx.h"
#include "WinOrLoseUI.h"
#include "../Utility/DataExtracter.h"
#include "../Manager/SpriteDataManager.h"

namespace basecross {
	WinOrLoseUI::WinOrLoseUI(const shared_ptr<Stage>& stage)
		:UIObjectBase(stage, L"WinOrLoseUI")
	{}

	WinOrLoseUI::WinOrLoseUI(const shared_ptr<Stage>& stage, const wstring& line)
		: UIObjectBase(stage, L"WinOrLoseUI")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		DataExtracter::RectTransformDataExtraction(tokens, m_rectTransformData);
	}
	void WinOrLoseUI::OnCreate() {
		const auto& stage = GetStage();

		auto spriteData = SpriteDataManager::GetInstance()->GetSpriteData(L"EastTeam");
		auto team = stage->AddGameObject<SimpleSprite>(SimpleSprite::Type::SpriteData, L"EastTeam");
		team->SetParent(GetThis<WinOrLoseUI>());
		auto rectTrans = team->GetComponent<RectTransform>();
		rectTrans->SetPosition(Vec2(0.0f, -spriteData.size.y));

		spriteData = SpriteDataManager::GetInstance()->GetSpriteData(L"EastTeam");
		auto winOrLose = stage->AddGameObject<SimpleSprite>(SimpleSprite::Type::SpriteData, L"Lose");
		winOrLose->SetParent(GetThis<WinOrLoseUI>());
		rectTrans = team->GetComponent<RectTransform>();
		rectTrans->SetPosition(Vec2(0.0f, +spriteData.size.y));
	}
}