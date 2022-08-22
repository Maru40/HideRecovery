#include "stdafx.h"
#include "CountDownUI.h"
#include "../Utility/Utility.h"
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
		// 切り捨ての関係上+1
		m_presentTime = m_startTime + 1;

		m_number->SetActive(false);
	}

	void CountDownUI::OnUpdate() {
		if (m_presentTime <= 1.0f) {
			m_isTimeUp = true;
			m_number->SetActive(false);
			return;
		}

		float delta = App::GetApp()->GetElapsedTime();
		// 現在時間を1～m_startTimeの間でクランプ
		float displayNum = Utility::Clamp(m_presentTime, 1.0f, m_startTime);
		m_number->SetValue((int)floorf(displayNum));
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