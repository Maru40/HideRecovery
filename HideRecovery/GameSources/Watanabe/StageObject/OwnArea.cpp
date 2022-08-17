#include "stdafx.h"
#include "OwnArea.h"
#include "../Utility/DataExtracter.h"

namespace basecross {
	OwnArea::OwnArea(const shared_ptr<Stage>& stage)
		:StageObjectBase(stage, L"OwnArea")
	{}
	OwnArea::OwnArea(const shared_ptr<Stage>& stage, const wstring& line)
		: StageObjectBase(stage, L"OwnArea")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		size_t nextIndex = DataExtracter::TransformDataExtraction(tokens, m_transformData);

		m_areaRadius = (float)_wtof(tokens[nextIndex + 0].c_str());

		wstring teamType = tokens[nextIndex + 1];
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
				L"OwnArea::OwnArea()"
			);
		}
	}

	void OwnArea::OnCreate() {
	}
	void OwnArea::OnUpdate() {
	}
}