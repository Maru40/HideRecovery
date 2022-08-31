/*!
@file   FadeComponent.cpp
@brief  フェードコンポーネント実体
 */

#include "stdafx.h"
#include "FadeComponent.h"

namespace basecross {
	void FadeComponent::OnCreate() {
		auto draw = GetGameObject()->GetDynamicComponent<SpriteBaseDraw>();
		auto color = draw->GetDiffuse();

		SetFadeRange(color.w, 0);
	}

	void FadeComponent::OnUpdate() {
		// 停止中は何もしない
		if (!m_bActive) {
			return;
		}

		auto draw = GetGameObject()->GetDynamicComponent<SpriteBaseDraw>();
		auto delta = App::GetApp()->GetElapsedTime();

		// フェードの方向に応じた処理
		switch (m_dir)
		{
		case Direction::FadeIn:
			m_delta += +delta;
			break;
		case Direction::FadeOut:
			m_delta += -delta;
			break;
		default:
			break;
		}

		if (m_delta >= 0 && m_delta <= m_fadeTime) {
			// フェードの処理
			auto alpha = Lerp::CalculateLerp(m_minAlpha, m_maxAlpha, 0, m_fadeTime, m_delta, Lerp::rate::Linear);
			auto color = draw->GetDiffuse();
			color.w = alpha;
			draw->SetDiffuse(color);
		}
		else // フェード終了後
		{
			auto color = draw->GetDiffuse();
			// 方向に応じて値をクランプ
			switch (m_dir)
			{
			case Direction::FadeIn:
				color.w = m_maxAlpha;
				break;
			case Direction::FadeOut:
				color.w = m_minAlpha;
				break;
			default:
				break;
			}
			draw->SetDiffuse(color);
			m_bActive = false;
			m_delta = 0.0f;
		}
	}

	void FadeComponent::FadeOut() {
		m_delta = m_fadeTime;
		m_dir = Direction::FadeOut;
		m_bActive = true;
	}

	void FadeComponent::FadeIn() {
		m_delta = 0.0f;
		m_dir = Direction::FadeIn;
		m_bActive = true;
	}
}
//end basecross