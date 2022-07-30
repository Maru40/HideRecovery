#pragma once
#include "stdafx.h"
#include "Number.h"

namespace basecross
{
	class Score : public GameObject
	{
		Col4 m_color = Col4(1.0f);

		int m_scoreBase;
		int m_score;
		float m_count;

		std::vector<std::shared_ptr<Number>> numbers;

	public:
		Score(const std::shared_ptr<Stage>& stage)
			:GameObject(stage), m_scoreBase(500000), m_score(0), m_count(0.0f)
		{
		}

		void OnCreate() override;
		void OnUpdate() override;
		void OnDraw() override;

	};

	class ResultRank : public GameObject
	{
		Col4 m_color = Col4(1.0f);
		int m_score;
		int m_scoreBase;
		int m_rankNum;

		std::vector<std::shared_ptr<HideRank>> ranknumbers;
	public:
		ResultRank(const std::shared_ptr<Stage>& stage)
			:GameObject(stage), m_score(0), m_scoreBase(500000), m_rankNum(0)
		{

		}

		void OnCreate() override;
		void OnUpdate() override;
		void OnDraw() override;
	};
}