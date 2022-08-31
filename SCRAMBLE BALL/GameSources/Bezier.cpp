#include"Bezier.h"
#include<cmath>

namespace itbs
{
	namespace Math
	{
		// BezierPoint ----------------------------------

		BezierPoint::BezierPoint() :
			BezierPoint(0, 0)
		{

		}

		BezierPoint::BezierPoint(const float time, const float value) :
			time(time),
			value(value)
		{

		}

		bool BezierPoint::operator==(const BezierPoint& other) const
		{
			return time == other.time &&
				value == other.value;
		}

		bool BezierPoint::operator!=(const BezierPoint& other) const
		{
			return !(*this == other);
		}

		BezierPoint BezierPoint::operator+(const BezierPoint& other) const
		{
			BezierPoint point;
			point.time = time + other.time;
			point.value = value + other.value;

			return point;
		}

		// BezierAnchorPoint ------------------------------------

		bool BezierAnchorPoint::IsLeftValid() const
		{
			return leftHandle != point;
		}

		bool BezierAnchorPoint::IsRightVaild() const
		{
			return rightHandle != point;
		}

		void BezierAnchorPoint::SetLeftHandleFromRadian(const float length,const float radian)
		{
			float x = std::cosf(radian) * length;
			float y = std::sinf(radian) * length;

			leftHandle = BezierPoint(-x, -y);
		}

		void BezierAnchorPoint::SetRightHandleFromRadian(const float length,const float radian)
		{
			float x = std::cosf(radian) * length;
			float y = std::sinf(radian) * length;

			rightHandle = BezierPoint(x, y);
		}

		void BezierAnchorPoint::SetDefaultHandle()
		{
			SetLeftHandleFromRadian(1.0f, 0.0f);
			SetRightHandleFromRadian(1.0f, 0.0f);
		}

		void BezierAnchorPoint::DeleteLeftHandle()
		{
			leftHandle = BezierPoint(0, 0);
		}

		void BezierAnchorPoint::DeleteRightHandle()
		{
			rightHandle = BezierPoint(0, 0);
		}

		// BezierManager ------------------------------------

		BezierManager::BezierManager(const float startValue)
		{
			AddAnchorPoint(0.0f, startValue);
		}

		float BezierManager::GetTime(const float time, const BezierPoint& p1, const BezierPoint& p2) const
		{
			float t = (time - p1.time) / (p2.time - p1.time);
			return p1.value * (1 - t) + p2.value * t;
		}

		float BezierManager::GetTime(const float time, const BezierPoint& p1, 
			const BezierPoint& p2, const BezierPoint& p3) const
		{
			float t = (time - p1.time) / (p3.time - p1.time);
			return p1.value * std::powf(1 - t, 2) +
				p2.value * 2 * t * (1 - t) +
				p3.value * std::powf(t, 2);
		}

		float BezierManager::GetTime(const float time, const BezierPoint& p1, 
			const BezierPoint& p2, const BezierPoint& p3, const BezierPoint& p4) const
		{
			float t = (time - p1.time) / (p4.time - p1.time);
			return p1.value * std::powf(1 - t, 3) +
				p2.value * 3 * t * std::powf(1 - t, 2) +
				p3.value * 3 * std::powf(t, 2) * (1 - t) +
				p4.value * std::powf(t, 3);
		}

		BezierAnchorPoint::BezierAnchorPoint(const BezierPoint& anchorPoint) :
			point(anchorPoint),
			leftHandle(anchorPoint),
			rightHandle(anchorPoint)
		{

		}

		BezierAnchorPoint::BezierAnchorPoint(const float time, const float value) :
			BezierAnchorPoint(BezierPoint(time, value))
		{

		}

		BezierAnchorPoint& BezierManager::AddAnchorPoint(const BezierPoint& bezierPoint)
		{
			m_anchorPoints.insert(std::make_pair(bezierPoint.time, BezierAnchorPoint(bezierPoint)));

			return m_anchorPoints.at(bezierPoint.time);
		}

		BezierAnchorPoint& BezierManager::AddAnchorPoint(const BezierAnchorPoint& bezierAnchorPoint)
		{
			m_anchorPoints.insert(std::make_pair(bezierAnchorPoint.point.time, bezierAnchorPoint));
			return m_anchorPoints.at(bezierAnchorPoint.point.time);
		}

		BezierAnchorPoint& BezierManager::AddAnchorPoint(const float time, const float value)
		{
			return AddAnchorPoint(BezierPoint(time, value));
		}

		BezierAnchorPoint& BezierManager::GetZeroAnchorPoint()
		{
			return m_anchorPoints.at(0.0f);
		}

		float BezierManager::GetLastPointTime() const
		{
			return m_anchorPoints.rbegin()->first;
		}

		float BezierManager::TimeToValue(const float time) const
		{

			auto it = m_anchorPoints.upper_bound(time);

			if (it == m_anchorPoints.end())
			{
				it--;
				return it->second.point.value;
			}

			const auto& endAnchor = it->second;
			it--;
			const auto& startAnchor = it->second;

			if (startAnchor.IsRightVaild() && endAnchor.IsLeftValid())
			{
				return GetTime(time, startAnchor.point, startAnchor.point + startAnchor.rightHandle,
					endAnchor.point + endAnchor.leftHandle, endAnchor.point);
			}

			if (startAnchor.IsRightVaild())
			{
				return GetTime(time, startAnchor.point, startAnchor.point + startAnchor.rightHandle, endAnchor.point);
			}

			if (endAnchor.IsLeftValid())
			{
				return GetTime(time, startAnchor.point, endAnchor.point + endAnchor.leftHandle, endAnchor.point);
			}

			return GetTime(time, startAnchor.point, endAnchor.point);
		}
	}

	namespace Utility
	{
		// TimeLineFloat --------------------------------

		TimeLineFloat::TimeLineFloat(const Math::BezierManager& bezier, const float maxTime) :
			m_bezier(bezier),
			m_maxTime(std::fmaxf(maxTime,0.01f))
		{

		}

		void TimeLineFloat::SetNowTime(const float nowTime)
		{
			m_nowTime = std::fminf(nowTime, m_maxTime);
			m_nowTime = std::fmaxf(m_nowTime, 0.0f);
		}

		void TimeLineFloat::SetMaxTime(const float maxTime)
		{
			m_maxTime = std::fmaxf(maxTime, 0.01f);
			m_nowTime = std::fminf(m_nowTime, m_maxTime);
		}

		void TimeLineFloat::SetBezierManager(const Math::BezierManager& bezier)
		{
			m_bezier = bezier;
		}

		void TimeLineFloat::SetIsReverse(const bool isReverse)
		{
			m_isReverse = isReverse;
		}

		void TimeLineFloat::AddTime(const float time)
		{
			float addTime = m_isReverse ? -time : time;
			m_nowTime = std::fminf(m_nowTime + addTime, m_maxTime);
			m_nowTime = std::fmaxf(m_nowTime, 0.0f);
		}

		float TimeLineFloat::GetValue() const
		{
			return m_bezier.TimeToValue(m_nowTime / m_maxTime);
		}

		bool TimeLineFloat::IsFinish() const
		{
			if (m_isReverse)
			{
				return m_nowTime <= 0.0f;
			}
			else
			{
				return m_nowTime >= m_maxTime;
			}
		}

		void TimeLineFloat::TimeReset()
		{
			m_nowTime = m_isReverse ? m_maxTime : 0.0f;
		}
	}
}