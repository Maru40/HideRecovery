
#pragma once
#include "stdafx.h"

namespace basecross
{
	class EnergeyGauge : public GameObject
	{
		float m_Egauge_x;    // ゲージ量
		float m_Egauge_MAX; // ゲージの最大の長さ
		float m_gauge_MAX;   // ゲージの最大値
		float m_counttime; // ゲージ点滅時に必要な計測時間


	public:
		EnergeyGauge(const std::shared_ptr<Stage>& stage)
			:GameObject(stage), m_Egauge_x(500.0f), m_Egauge_MAX(500.0f), m_gauge_MAX(500.0f), m_counttime(0.0f)
		{

		}

		void OnCreate() override;
		void OnUpdate() override;
		//void OnDraw();

	};

}