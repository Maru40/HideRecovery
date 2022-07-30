#pragma once
#include "stdafx.h"

namespace basecross
{
	class DamageGauge : public GameObject
	{
		float m_DamageGauge_x;    // ゲージ量
		float m_DamageGauge_MAX; // ゲージの最大の長さ
		float m_Gauge_MAX;   // ゲージの最大値


	public:
		DamageGauge(const shared_ptr<Stage>& stage)
			:GameObject(stage), m_DamageGauge_x(215.0f), m_DamageGauge_MAX(215.0f), m_Gauge_MAX(215.0f)
		{
		}	

	    void OnCreate() override;
	    void OnUpdate() override;
		void OnDraw() override;

	};

}
