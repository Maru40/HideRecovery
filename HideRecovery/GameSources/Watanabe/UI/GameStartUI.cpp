#include "stdafx.h"
#include "GameStartUI.h"
#include "../Component/LifeSpan.h"

namespace basecross {
	GameStartUI::GameStartUI(const shared_ptr<Stage>& stage)
		:GameObject(stage), m_isStart(false), m_invisibleTimer(1)
	{}

	void GameStartUI::OnCreate() {
		const float countTime = 3.0f;

		const auto& stage = GetStage();
		m_countDown = stage->AddGameObject<CountDownUI>(countTime);
		m_countDown->SetActive(false);

		m_strStart = stage->AddGameObject<SimpleSprite>(
			SimpleSprite::Type::SpriteData, L"Start");
		m_strStart->SetActive(false);

		m_countDown->SetScale(2);
		m_countDown->SetColor(Col4(1, 0.5f, 0, 1));

		m_strStart->GetComponent<Transform>()->SetScale(Vec3(2));
		m_strStart->GetDrawComponent()->SetDiffuse(Col4(1, 0.5f, 0, 1));
	}
	void GameStartUI::OnUpdate() {
		if (!m_isStart)
			return;

		if (!m_isBeforeTimeUp && m_countDown->IsTimeUp())
		{
			for (auto& timeUpEventFunc : m_timeUpEventFuncs)
			{
				timeUpEventFunc();
			}
		}

		if (m_countDown->IsTimeUp()) {
			m_strStart->SetDrawActive(true);
			if (m_invisibleTimer.Count()) {
				m_strStart->SetActive(false);
				m_countDown->SetActive(false);
			}
		}

		m_isBeforeTimeUp = m_countDown->IsTimeUp();
	}

	void GameStartUI::Start() {
		m_isStart = true;
		m_isBeforeTimeUp = false;
		m_countDown->SetActive(true);
		m_countDown->Start();
	}

	void GameStartUI::Reset() {
		m_isStart = false;
		m_isBeforeTimeUp = false;
		m_countDown->SetActive(true);
		m_countDown->Reset();
		m_invisibleTimer.Reset();
	}

	bool GameStartUI::IsGameBegin() {
		return m_countDown->IsTimeUp();
	}
}