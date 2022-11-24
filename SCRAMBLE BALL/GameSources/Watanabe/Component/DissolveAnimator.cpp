﻿#include "stdafx.h"
#include "DissolveAnimator.h"

namespace basecross {
	DissolveAnimator::DissolveAnimator(const shared_ptr<GameObject>& owner)
		:Component(owner), m_timer(1)
	{}

	void DissolveAnimator::OnLateStart() {
		// このコンポーネントはAdvBaseDraw必須
		m_drawComp = GetGameObject()->GetComponent<AdvBaseDraw>();
	}
	void DissolveAnimator::OnUpdate() {
		if (!m_isPlayingAnimation)
			return;

		if (m_timer.IsTimeUp()) {
			m_isPlayingAnimation = false;
			m_drawComp.lock()->SetDissolveAnimationRate(0.0f);
			if (m_playEndEvent)
				m_playEndEvent();
			return;
		}

		m_drawComp.lock()->SetDissolveAnimationRate(m_timer.GetInverseTimeRate());
		m_timer.Count();
	}

	bool DissolveAnimator::IsPlayEnd() {
		return m_timer.IsTimeUp();
	}

	void DissolveAnimator::Start() {
		m_isPlayingAnimation = true;
		m_timer.Reset();
	}
	void DissolveAnimator::Reset() {
		m_drawComp.lock()->SetDissolveAnimationRate(1.0f);
	}

	void DissolveAnimator::SetPlayEndEvent(const function<void()>& func) {
		m_playEndEvent = func;
	}

	void DissolveAnimator::SetPlayLength(float length) {
		m_timer.SetIntervalTime(length);
	}
}