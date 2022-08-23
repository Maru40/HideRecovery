#include "stdafx.h"
#include "PointUI.h"
#include "../Manager/PointManager.h"

namespace basecross {
	PointUI::PointUI(const shared_ptr<Stage>& stage)
		:GameObject(stage), m_team(Team::East)
	{}

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