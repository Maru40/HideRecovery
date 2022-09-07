#include "stdafx.h"
#include "ScoreUI.h"
#include "../Utility/DataExtracter.h"
#include "../Manager/ScoreManager.h"
#include "../UI/SimpleSprite.h"
#include "../UI/Numbers.h"
#include "../UI/UIObjectCSVBuilder.h"
#include "../DebugClass/Debug.h"
#include "Maruyama/Utility/Utility.h"

namespace basecross {
	ScoreUI::ScoreUI(const shared_ptr<Stage>& stage, int playerNumber, team::TeamType teamType)
		:UIObjectBase(stage, L"ScoreUI"), m_playerNumber(playerNumber), m_teamType(teamType)
	{}

	ScoreUI::ScoreUI(const shared_ptr<Stage>& stage, const wstring& line)
		: UIObjectBase(stage, L"ScoreUI")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		size_t nextIndex = DataExtracter::RectTransformDataExtraction(tokens, m_rectTransformData);
		m_playerNumber = stoi(tokens[nextIndex]);

		wstring teamType = tokens[nextIndex + 1];
		if (teamType == L"Blue") {
			m_teamType = team::TeamType::Blue;
		}
		else if (teamType == L"Red") {
			m_teamType = team::TeamType::Red;
		}
		else {
			throw BaseException(
				L"Teamが不正な値です。",
				L"team::TeamType : " + teamType,
				L"PointUI::PointUI()"
			);
		}
		m_gamePlayerNumber = stoi(tokens[nextIndex + 2]);
	}

	void ScoreUI::OnCreate() {
		const auto scoreData = ScoreManager::GetInstance()->GetScoreData(m_gamePlayerNumber);
		const auto& stage = GetStage();

		// UIパーツ生成
		auto uiBuilder = UIObjectCSVBuilder::Create();
		uiBuilder->Register<SimpleSprite>(L"SimpleSprite");
		uiBuilder->Register<Numbers>(L"Numbers");
		auto dir = App::GetApp()->GetDataDirWString();
		auto path = dir + L"CSVDatas/";
		uiBuilder->Build(GetStage(), path + L"ScoreUILayout.csv", GetThis<ScoreUI>());

		// データの適用
		auto playerNumberUI = uiBuilder->GetUIObject<Numbers>(L"PlayerNumber");
		playerNumberUI->SetNumber(m_playerNumber);
		auto killCountNumber = uiBuilder->GetUIObject<Numbers>(L"KillCountNumber");
		killCountNumber->SetNumber(scoreData.KillCount);
		auto goalCountNumber = uiBuilder->GetUIObject<Numbers>(L"GoalCountNumber");
		goalCountNumber->SetNumber(scoreData.GoalCount);

		// チーム別に変更
		wstring teamStr = L"";
		switch (m_teamType)
		{
		case team::TeamType::Blue:
			teamStr = L"Blue";
			break;
		case team::TeamType::Red:
			teamStr = L"Red";
			break;
		}
		auto label = uiBuilder->GetUIObject<SimpleSprite>(L"PlayerLabel");
		label->ChangeSprite(SimpleSprite::Type::SpriteData, teamStr);

		// 色の変更
		const auto TeamColor = team::GetTeamColor(m_teamType);
		auto frameUI = uiBuilder->GetUIObject<SimpleSprite>(L"Background");
		frameUI->GetDrawComponent()->SetDiffuse(TeamColor);
	}
}