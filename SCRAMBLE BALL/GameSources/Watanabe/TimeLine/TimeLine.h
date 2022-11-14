#pragma once
#include "stdafx.h"
#include "KeyFrameData.h"
#include "AdvQueue.h"

namespace basecross {
	class TimeLine :public Component {
	public:
		struct EventData {
			float Time;
			function<void()> Func;

			EventData() {}
			EventData(float time, const function<void()>& func)
				:Time(time), Func(func)
			{}
		};
	private:
		bool m_isPlaying = false;
		float m_delta;
		weak_ptr<Camera> m_camera;

		shared_ptr<CameraKeyFrameData> m_currentKey;
		shared_ptr<CameraKeyFrameData> m_nextKey;
		vector<shared_ptr<CameraKeyFrameData>> m_keyFrameList;
		AdvQueue<shared_ptr<CameraKeyFrameData>> m_timeLine;

		AdvQueue<EventData> m_eventList;
		EventData m_waitingEvent;
		bool m_endEvent = false;
	public:
		TimeLine(const shared_ptr<GameObject>& owner);

		void OnCreate()override;
		void OnUpdate()override;
		void OnDraw()override {}

		void AddKeyFrame(const CameraKeyFrameData data);
		void AddEvent(float time, const function<void()>& func);
		void Play();
		void Reset();
	};
}
