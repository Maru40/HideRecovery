#pragma once
#include "stdafx.h"
#include "Number.h"

namespace basecross
{
	class CountTime : public GameObject
	{
		Col4 m_color = Col4(1.0f);

		int m_time;
		float m_count;
		std::vector<std::shared_ptr<Number>> numbers;

	public:
		CountTime(const std::shared_ptr<Stage>& stage)
			:GameObject(stage), m_time(0), m_count(0.0f)
		{

		}

		void OnCreate() override;
		void OnUpdate() override;
		void OnDraw() override;
	};

	class CountTimeMinute : public CountTime
	{
		Col4 m_color = Col4(1.0f);

		int m_time;
		float m_count;
		std::vector<std::shared_ptr<Number>> numbers;

	public:
		CountTimeMinute(const std::shared_ptr<Stage>& stage)
			:CountTime(stage), m_time(0), m_count(0.0f)
		{

		}

		void OnCreate() override;
		void OnUpdate() override;
		void OnDraw() override;
	};

}