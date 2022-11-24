#pragma once
#include "stdafx.h"
#include "../Shader/AdvBaseDraw.h"
#include "../Utility/TimeCounter.h"

namespace basecross {
	class DissolveAnimator :public Component {
		// アニメーション再生中か
		bool m_isPlayingAnimation = false;
		TimeCounter m_timer;
		weak_ptr<AdvBaseDraw> m_drawComp;
		function<void()> m_playEndEvent;
	public:
		DissolveAnimator(const shared_ptr<GameObject>& owner);

		void OnCreate()override {}
		void OnLateStart()override;
		void OnUpdate()override;
		void OnDraw()override {}

		void Start();
		bool IsPlayEnd();
		void Reset();

		void SetPlayEndEvent(const function<void()>& func);
		void SetPlayLength(float length);
	};
}
