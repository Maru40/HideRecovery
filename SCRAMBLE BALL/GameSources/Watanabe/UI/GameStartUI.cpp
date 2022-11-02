#include "stdafx.h"
#include "GameStartUI.h"
#include "../Component/LifeSpan.h"
#include "../Utility/Utility.h"

namespace basecross {
	GameStartUI::GameStartUI(const shared_ptr<Stage>& stage)
		:GameObject(stage), m_isStart(false), m_invisibleTimer(3)
	{}

	void GameStartUI::OnCreate() {
		const float countTime = 3.0f;

		const auto& stage = GetStage();
		m_countDown = stage->AddGameObject<CountDownUI>(countTime);
		m_countDown->SetScale(2);
		m_countDown->SetColor(Col4(1, 0.5f, 0, 1));
		{
			m_strStart = stage->AddGameObject<SimpleSprite>(
				SimpleSprite::Type::SpriteData, L"Start");
			auto startSpriteData = SpriteDataManager::GetInstance()->GetSpriteData(L"Start");
			auto trans = m_strStart->GetComponent<RectTransform>();
			trans->SetScale(Vec2(2.0f));
			m_strStart->GetDrawComponent()->SetDiffuse(Col4(1, 0.5f, 0, 1));
		}
		{
			m_objectiveStr = stage->AddGameObject<SimpleSprite>(SimpleSprite::Type::Texture, L"Objective_TX");
			auto trans = m_objectiveStr->GetComponent<RectTransform>();
			trans->SetScale(Vec2(0.75f));
			auto objectiveTexSize = TextureUtility::GetTextureSize(L"Objective_TX");
			trans->SetPosition(Vec2(0, -objectiveTexSize.y));
		}
		m_countDown->SetActive(false);
		m_strStart->SetDrawActive(false);
		m_objectiveStr->SetDrawActive(false);
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
			m_objectiveStr->SetDrawActive(true);
			if (m_invisibleTimer.Count()) {
				m_strStart->SetDrawActive(false);
				m_countDown->SetActive(false);
				m_objectiveStr->SetDrawActive(false);
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