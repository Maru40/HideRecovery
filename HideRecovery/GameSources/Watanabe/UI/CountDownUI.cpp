#include "stdafx.h"
#include "CountDownUI.h"
#include "../DebugClass/Debug.h"

namespace basecross {
	CountDownUI::CountDownUI(const shared_ptr<Stage>& stage)
		:GameObject(stage), m_startTime(3), m_presentTime(0), m_isActive(true)
	{}

	void CountDownUI::OnCreate() {
		m_number = GetStage()->AddGameObject<NumberSprite>();
		// 切り捨ての関係上+1
		m_presentTime = m_startTime + 1;
	}
	void CountDownUI::OnUpdate() {
		// 無効の場合 または カウントダウンが終了時 何もしない
		if (!m_isActive || m_presentTime <= 1.0f) {
			m_number->SetDrawActive(false);
			return;
		}

		float delta = App::GetApp()->GetElapsedTime();

		m_number->SetValue((int)floorf(m_presentTime));

		m_presentTime -= delta;
		Debug::GetInstance()->Log(m_presentTime);
	}
}