#include "stdafx.h"
#include "WinOrLoseUI.h"
#include "../Utility/DataExtracter.h"
#include "../Manager/SpriteDataManager.h"
#include "../Manager/PointManager.h"

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
	}

	void WinOrLoseUI::SetTeam(team::TeamType team) {
		wstring teamStr = L"";
		switch (team)
		{
		case team::TeamType::East:
			teamStr = L"EastTeam";
			break;
		case team::TeamType::West:
			teamStr = L"WestTeam";
			break;
		}

		auto winOrLose = PointManager::GetInstance()->GetSelfTeamWinOrLose(team);

		wstring winOrLoseStr = L"";
		switch (winOrLose)
		{
		case PointManager::WinOrLose::Win:
			winOrLoseStr = L"Win";
			break;
		case PointManager::WinOrLose::Draw:
			winOrLoseStr = L"Draw";
			break;
		case PointManager::WinOrLose::Lose:
			winOrLoseStr = L"Lose";
			break;
		}

		const auto& stage = GetStage();
		// 先に勝敗を表示
		auto spriteData = SpriteDataManager::GetInstance()->GetSpriteData(winOrLoseStr);
		auto winOrLoseSprite = stage->AddGameObject<SimpleSprite>(SimpleSprite::Type::SpriteData, winOrLoseStr);
		winOrLoseSprite->SetParent(GetThis<WinOrLoseUI>());

		// Draw以外ならチームも表示
		if (winOrLose != PointManager::WinOrLose::Draw) {
			// チーム表示
			auto spriteData = SpriteDataManager::GetInstance()->GetSpriteData(teamStr);
			auto teamSprite = stage->AddGameObject<SimpleSprite>(SimpleSprite::Type::SpriteData, teamStr);
			teamSprite->SetParent(GetThis<WinOrLoseUI>());
			auto rectTrans = teamSprite->GetComponent<RectTransform>();
			rectTrans->SetPosition(Vec2(0.0f, +spriteData.size.y / 2.0f));

			// 勝敗表示
			rectTrans = winOrLoseSprite->GetComponent<RectTransform>();
			rectTrans->SetPosition(Vec2(0.0f, -spriteData.size.y / 2.0f));
		}
	}
}