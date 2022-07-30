#pragma once
#include "stdafx.h"
#include "Number.h"

namespace basecross
{
	class ResultTime : public GameObject
	{
		Col4 m_color = Col4(1.0f);

		int m_time;
		float m_count;
		std::vector<std::shared_ptr<Number>> numbers;

	public:
		ResultTime(const std::shared_ptr<Stage>& stage)
			:GameObject(stage), m_time(0), m_count(0.0f)
		{

		}

		void OnCreate() override;
		void OnUpdate() override;
		void OnDraw() override;
	};

	class ResultTimeMinute : public ResultTime
	{
		Col4 m_color = Col4(1.0f);

		int m_time;
		float m_count;
		std::vector<std::shared_ptr<Number>> numbers;

	public:
		ResultTimeMinute(const std::shared_ptr<Stage>& stage)
			:ResultTime(stage), m_time(0), m_count(0.0f)
		{

		}

		void OnCreate() override;
		void OnUpdate() override;
		void OnDraw() override;
	};
}