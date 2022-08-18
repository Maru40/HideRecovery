#include "stdafx.h"
#include "GameStartUI.h"

namespace basecross {
	GameStartUI::GameStartUI(const shared_ptr<Stage>& stage)
		:GameObject(stage)
	{}

	void GameStartUI::OnCreate() {
		const auto& stage = GetStage();
		m_countDown = stage->AddGameObject<CountDownUI>();
		m_strStart = stage->AddGameObject<SimpleSprite>(
			SimpleSprite::Type::SpriteData, L"Start");

		m_countDown->Start();
		m_strStart->SetDrawActive(false);

		m_countDown->SetScale(2);
		m_countDown->SetColor(Col4(1, 0.5f, 0, 1));

		m_strStart->GetComponent<Transform>()->SetScale(Vec3(2));
		m_strStart->GetDrawComponent()->SetDiffuse(Col4(1, 0.5f, 0, 1));
	}
	void GameStartUI::OnUpdate() {
		if (m_countDown->IsTimeUp()) {
			m_strStart->SetDrawActive(true);
		}
	}

	void GameStartUI::OnDestroy() {
		const auto& stage = GetStage();
		stage->RemoveGameObject<CountDownUI>(m_countDown);
		stage->RemoveGameObject<SimpleSprite>(m_strStart);
	}

	bool GameStartUI::IsGameBegan() {
		return m_countDown->IsTimeUp();
	}
}