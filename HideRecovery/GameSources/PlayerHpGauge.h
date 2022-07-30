
#pragma once
#include "stdafx.h"

namespace basecross
{
	class PlayerHpgauge : public GameObject
	{
		float m_PHpgauge_x;    // ゲージ量
		float m_PHpgauge_MAX; // ゲージの最大の長さ
		float m_gauge_MAX;   // ゲージの最大値

	public:
		PlayerHpgauge(const std::shared_ptr<Stage>& stage)
			:GameObject(stage), m_PHpgauge_x(500.0f), m_PHpgauge_MAX(500.0f), m_gauge_MAX(500.0f)
		{

		}

		void OnCreate() override;
		void OnUpdate() override;
		void OnDraw();

	};

}