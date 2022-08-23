#include "stdafx.h"
#include "PointUI.h"
#include "../Manager/PointManager.h"
#include "../Utility/DataExtracter.h"

namespace basecross {
	PointUI::PointUI(const shared_ptr<Stage>& stage)
		:UIObjectBase(stage, L"PointUI"), m_team(Team::East)
	{}

	PointUI::PointUI(const shared_ptr<Stage>& stage, const wstring& line)
		: UIObjectBase(stage, L"PointUI")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		auto nextIndex = DataExtracter::RectTransformDataExtraction(tokens, m_rectTransformData);
		// 仮で入れる
		m_team = Team::East;
		wstring teamType = tokens[nextIndex];
		if (teamType == L"East") {
			m_team = Team::East;
		}
		else if (teamType == L"West") {
			m_team = Team::West;
		}
		else {
			throw BaseException(
				L"Teamが不正な値です。",
				L"Team : " + teamType,
				L"PointUI::PointUI()"
			);
		}
	}

	void PointUI::OnCreate() {
		m_numbers = GetStage()->AddGameObject<Numbers>(2);
		m_numbers->SetParent(GetThis<PointUI>());
	}
	void PointUI::OnUpdate() {
		int point = PointManager::GetInstance()->GetPoint(m_team);
		m_numbers->SetNumber(point);
	}
}