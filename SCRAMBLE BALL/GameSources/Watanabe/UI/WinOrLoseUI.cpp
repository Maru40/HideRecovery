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
		wstring winOrLoseStr = L"";
		if (PointManager::GetInstance()->IsDraw()) {
			winOrLoseStr = L"Draw";
		}
		else {
			winOrLoseStr = L"Win";
		}

		const auto& stage = GetStage();
		// 先に勝敗を表示
		auto wolSpriteData = SpriteDataManager::GetInstance()->GetSpriteData(winOrLoseStr);
		auto winOrLoseSprite = stage->AddGameObject<SimpleSprite>(SimpleSprite::Type::SpriteData, winOrLoseStr);
		winOrLoseSprite->SetParent(GetThis<WinOrLoseUI>());

		// Draw以外ならチームも表示
		if (winOrLoseStr != L"Draw") {
			auto winner = PointManager::GetInstance()->GetWinner();
			wstring teamStr = L"";
			switch (winner)
			{
			case team::TeamType::Blue:
				teamStr = L"BlueTeam";
				break;
			case team::TeamType::Red:
				teamStr = L"RedTeam";
				break;
			}
			// チーム表示
			auto teamSpriteData = SpriteDataManager::GetInstance()->GetSpriteData(teamStr);
			auto teamSprite = stage->AddGameObject<SimpleSprite>(SimpleSprite::Type::SpriteData, teamStr);
			teamSprite->SetParent(GetThis<WinOrLoseUI>());
			teamSprite->GetDrawComponent()->SetDiffuse(team::GetTeamColor(winner));

			// それぞれのスプライトサイズのXの半分の値を計算
			auto halfTeamSprite = teamSpriteData.size.x / 2.0f;
			auto halfWinOrLoseSprite = wolSpriteData.size.x / 2.0f;

			auto rectTrans = teamSprite->GetComponent<RectTransform>();
			rectTrans->SetPosition(Vec2(-halfWinOrLoseSprite, 0));
			rectTrans = winOrLoseSprite->GetComponent<RectTransform>();
			rectTrans->SetPosition(Vec2(halfTeamSprite, 0));
		}
	}

	void WinOrLoseUI::SetTeam(team::TeamType team) {
		wstring teamStr = L"";
		switch (team)
		{
		case team::TeamType::Blue:
			teamStr = L"BlueTeam";
			break;
		case team::TeamType::Red:
			teamStr = L"RedTeam";
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
			teamSprite->GetDrawComponent()->SetDiffuse(team::GetTeamColor(team));
			auto rectTrans = teamSprite->GetComponent<RectTransform>();
			rectTrans->SetPosition(Vec2(0.0f, spriteData.size.y));
		}
	}
}