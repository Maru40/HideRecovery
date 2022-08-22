#include "stdafx.h"
#include "GameFinishUI.h"

namespace basecross {
	GameFinishUI::GameFinishUI(const shared_ptr<Stage>& stage)
		:GameObject(stage), m_isStart(false)
	{}

	void GameFinishUI::OnCreate() {
		const float countTime = 5.0f;

		const auto& stage = GetStage();
		m_countDown = stage->AddGameObject<CountDownUI>(countTime);
		m_countDown->SetActive(false);

		m_strFinish = stage->AddGameObject<SimpleSprite>(
			SimpleSprite::Type::SpriteData, L"Finish");
		m_strFinish->SetActive(false);

		m_countDown->SetScale(2);
		m_countDown->SetColor(Col4(1, 0.5f, 0, 1));

		m_strFinish->GetComponent<Transform>()->SetScale(Vec3(2));
		m_strFinish->GetDrawComponent()->SetDiffuse(Col4(1, 0.5f, 0, 1));
	}
	void GameFinishUI::OnUpdate() {
		if (!m_isStart)
			return;

		if (m_countDown->IsTimeUp()) {
			m_strFinish->SetDrawActive(true);
		}
	}

	void GameFinishUI::Start() {
		m_isStart = true;
		m_countDown->SetActive(true);
		m_countDown->Start();
	}

	void GameFinishUI::Reset() {
		m_isStart = false;
		m_countDown->SetActive(true);
		m_countDown->Reset();
	}

	bool GameFinishUI::IsFinish() {
		return m_countDown->IsTimeUp();
	}
}