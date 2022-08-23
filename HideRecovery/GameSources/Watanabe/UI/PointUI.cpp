#include "stdafx.h"
#include "PointUI.h"
#include "../Manager/PointManager.h"
#include "../Utility/DataExtracter.h"

namespace basecross {
	PointUI::PointUI(const shared_ptr<Stage>& stage)
		:StageObjectBase(stage, L"PointUI"), m_team(Team::East)
	{}

	PointUI::PointUI(const shared_ptr<Stage>& stage, const wstring& line)
		: StageObjectBase(stage, L"PointUI")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		DataExtracter::TransformDataExtraction(tokens, m_transformData);
		// 仮で入れる
		m_team = Team::East;
	}

	void PointUI::OnCreate() {
		m_numbers = GetStage()->AddGameObject<Numbers>(2);
		auto numbersTrans = m_numbers->GetComponent<Transform>();
		numbersTrans->SetPosition(transform->GetPosition());
		numbersTrans->SetParent(GetThis<PointUI>());
	}
	void PointUI::OnUpdate() {
		int point = PointManager::GetInstance()->GetPoint(m_team);
		m_numbers->SetNumber(point);
	}
}