#include "stdafx.h"
#include "TimeLine.h"
#include "../DebugClass/Debug.h"

namespace basecross {
	TimeLine::TimeLine(const shared_ptr<GameObject>& owner)
		:Component(owner)
	{}

	void TimeLine::OnCreate() {
		m_camera = GetStage()->GetView()->GetTargetCamera();
	}
	void TimeLine::OnUpdate() {
		if (!m_isPlaying)
			return;

		if (m_delta > m_nextKey->Time) {
			m_currentKey = m_nextKey;
			if (m_timeLine.empty()) {
				return;
			}
			m_nextKey = m_timeLine.front();
			m_timeLine.pop();
		}

		auto data = m_currentKey->Interpolation(m_nextKey, m_delta);

		m_camera.lock()->SetEye(data.Eye);
		m_camera.lock()->SetAt(data.At);

		if (m_delta > m_waitingEvent.Time && !m_endEvent) {
			if (m_waitingEvent.Func)
				m_waitingEvent.Func();

			if (!m_eventList.empty()) {
				m_waitingEvent = m_eventList.front();
				m_eventList.pop();
			}
			else {
				m_endEvent = true;
			}
		}

		m_delta += App::GetApp()->GetElapsedTime();
	}

	void TimeLine::AddKeyFrame(const CameraKeyFrameData data) {
		m_timeLine.push(make_shared<CameraKeyFrameData>(data));
	}

	void TimeLine::AddEvent(float time, const function<void()>& func) {
		m_eventList.push(EventData(time, func));
	}

	void TimeLine::Play() {
		//sort(m_keyFrameList.begin(), m_keyFrameList.end());
		m_currentKey = m_timeLine.front();
		m_timeLine.pop();

		m_nextKey = m_timeLine.front();
		m_timeLine.pop();

		m_isPlaying = true;

		m_waitingEvent = m_eventList.front();
		m_eventList.pop();
		m_endEvent = false;
	}
	void TimeLine::Reset() {
		m_delta = 0.0f;
	}
}