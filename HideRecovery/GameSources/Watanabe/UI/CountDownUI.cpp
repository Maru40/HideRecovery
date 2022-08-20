#include "stdafx.h"
#include "CountDownUI.h"
#include "../DebugClass/Debug.h"

namespace basecross {
	CountDownUI::CountDownUI(const shared_ptr<Stage>& stage)
		:CountDownUI(stage, 3)
	{}

	CountDownUI::CountDownUI(const shared_ptr<Stage>& stage, float countTime)
		: GameObject(stage), m_startTime(countTime), m_presentTime(0), m_isTimeUp(false)
	{}

	void CountDownUI::OnCreate() {
		m_number = GetStage()->AddGameObject<NumberSprite>();
		m_number->GetComponent<Transform>()->SetParent(GetThis<CountDownUI>());
		m_number->SetValue((int)floorf(m_startTime));
		Debug::GetInstance()->Log((int)floorf(m_startTime));
		// 切り捨ての関係上+1
		m_presentTime = m_startTime + 1;

		m_number->SetActive(false);
	}

	void CountDownUI::OnUpdate() {
		if (m_presentTime <= 1.0f) {
			m_number->SetActive(false);
			m_isTimeUp = true;
			return;
		}

		float delta = App::GetApp()->GetElapsedTime();
		m_number->SetValue((int)floorf(m_presentTime));
		m_presentTime -= delta;
	}

	void CountDownUI::Start() {
		m_number->SetActive(true);
	}

	void CountDownUI::Reset() {
		m_isTimeUp = false;
		m_presentTime = m_startTime + 1;
		m_number->SetActive(false);
	}

	bool CountDownUI::IsTimeUp() {
		return m_isTimeUp;
	}

	void CountDownUI::SetScale(float size) {
		m_number->GetComponent<Transform>()->SetScale(Vec3(size));
	}

	void CountDownUI::SetColor(Col4 color) {
		m_number->GetDrawComponent()->SetDiffuse(color);
	}
}