
#pragma once
#include "stdafx.h"

namespace basecross
{
	class EnergeyGauge : public GameObject
	{
		float m_Egauge_x;    // �Q�[�W��
		float m_Egauge_MAX; // �Q�[�W�̍ő�̒���
		float m_gauge_MAX;   // �Q�[�W�̍ő�l
		float m_counttime; // �Q�[�W�_�Ŏ��ɕK�v�Ȍv������


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