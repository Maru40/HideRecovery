#include "stdafx.h"
#include "ScoreUI.h"
#include "../Utility/DataExtracter.h"
#include "../Manager/ScoreManager.h"
#include "../UI/SimpleSprite.h"
#include "../UI/Numbers.h"
#include "../UI/UIObjectCSVBuilder.h"
#include "../DebugClass/Debug.h"

namespace basecross {
	ScoreUI::ScoreUI(const shared_ptr<Stage>& stage, int playerNumber)
		:UIObjectBase(stage, L"ScoreUI"), m_playerNumber(playerNumber)
	{}

	ScoreUI::ScoreUI(const shared_ptr<Stage>& stage, const wstring& line)
		: UIObjectBase(stage, L"ScoreUI")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		size_t nextIndex = DataExtracter::RectTransformDataExtraction(tokens, m_rectTransformData);
		m_playerNumber = stoi(tokens[nextIndex]);
	}

	void ScoreUI::OnCreate() {
		const auto scoreData = ScoreManager::GetInstance()->GetScoreData(m_playerNumber);
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
		playerNumberUI->SetNumber(m_playerNumber + 1);
		auto killCountNumber = uiBuilder->GetUIObject<Numbers>(L"KillCountNumber");
		killCountNumber->SetNumber(scoreData.KillCount);
		auto goalCountNumber = uiBuilder->GetUIObject<Numbers>(L"GoalCountNumber");
		goalCountNumber->SetNumber(scoreData.GoalCount);
	}
}