/*!
@file   OwnAreaObject.cpp
@brief  自陣エリアクラス実体
*/

#include "stdafx.h"
#include "OwnAreaObject.h"
#include "../Utility/DataExtracter.h"
#include "../DebugClass/Debug.h"

#include "Maruyama/Player/Component/OwnArea.h"

namespace basecross {
	OwnAreaObject::OwnAreaObject(const shared_ptr<Stage>& stage)
		:StageObjectBase(stage, L"OwnArea"), m_areaRadius(20), m_team(Team::East)
	{}
	OwnAreaObject::OwnAreaObject(const shared_ptr<Stage>& stage, const wstring& line)
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

	void OwnAreaObject::OnCreate() {
		Debug::GetInstance()->Log(L"Create OwnArea");

		AddComponent<OwnArea>(OwnArea::Parametor(m_team, m_areaRadius));
	}
	void OwnAreaObject::OnUpdate() {
	}
}