#pragma once
#include "stdafx.h"

namespace basecross
{
	class DamageGauge : public GameObject
	{
		float m_DamageGauge_x;    // �Q�[�W��
		float m_DamageGauge_MAX; // �Q�[�W�̍ő�̒���
		float m_Gauge_MAX;   // �Q�[�W�̍ő�l


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
