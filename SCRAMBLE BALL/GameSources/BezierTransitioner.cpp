#include"BezierTransitioner.h"

namespace basecross
{
	BezierTransitioner::FunctionPointPair::FunctionPointPair(func_type function, const BezierManager& manager) :
		function(function),
		pointManager(manager)
	{

	}
	
	BezierTransitioner::BezierTransitioner(std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{

	}

	void BezierTransitioner::AddManager(func_type function, const BezierManager& manager)
	{
		m_pointManagers.push_back(FunctionPointPair(function, manager));
		m_loopTime = std::fmaxf(m_loopTime, manager.GetLastPointTime());
	}

	void BezierTransitioner::Play()
	{
		m_isPlaying = true;
	}

	void BezierTransitioner::Stop()
	{
		m_isPlaying = false;
	}

	void BezierTransitioner::Skip(const float skipTime)
	{
		m_time += skipTime;
	}

	void BezierTransitioner::Move(const float time)
	{
		m_time = time;
	}

	void BezierTransitioner::MoveStart()
	{
		m_time = 0;
	}

	void BezierTransitioner::MoveEnd()
	{
		m_time = m_loopTime;
	}

	void BezierTransitioner::SetIsLoop(const bool isLoop)
	{
		m_isLoop = isLoop;
	}

	void BezierTransitioner::OnUpdate()
	{
		if (!m_isPlaying)
		{
			return;
		}

		for (auto& manager : m_pointManagers)
		{
			manager.function(manager.pointManager.TimeToValue(m_time));
		}

		m_time += App::GetApp()->GetElapsedTime();

		if (m_isLoop && m_time >= m_loopTime)
		{
			m_time -= m_loopTime;
		}
	}
}