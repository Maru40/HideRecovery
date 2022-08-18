#include "stdafx.h"
#include "CountDownUI.h"

namespace basecross {
	CountDownUI::CountDownUI(const shared_ptr<Stage>& stage)
		:GameObject(stage), m_startTime(3), m_presentTime(0), m_isActive(true)
	{}

	void CountDownUI::OnCreate() {
		m_number = GetStage()->AddGameObject<NumberSprite>();
		m_presentTime = m_startTime;
	}
	void CountDownUI::OnUpdate() {
		// 無効の場合 または カウントダウンが終了時 何もしない
		if (!m_isActive || m_presentTime <= 0.0f) {
			SetDrawActive(false);
			return;
		}

		float delta = App::GetApp()->GetElapsedTime();

		m_number->SetValue((int)floorf(m_presentTime));

		m_presentTime -= delta;
	}
}