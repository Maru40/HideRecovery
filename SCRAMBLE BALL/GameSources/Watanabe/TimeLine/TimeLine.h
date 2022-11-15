#pragma once
#include "stdafx.h"
#include "KeyFrameData.h"
#include "AdvQueue.h"

namespace basecross {
	namespace timeline {
		template<class TKeyFrame>
		class TimeLineData {
			shared_ptr<TKeyFrame> m_currentKey;
			shared_ptr<TKeyFrame> m_nextKey;
			vector<shared_ptr<TKeyFrame>> m_keyFrameList;
			AdvQueue<shared_ptr<TKeyFrame>> m_timeLine;
		};

		template<class T, class TKeyFrame>
		class TimeLine {
			bool m_isPlaying = false;
			float m_delta;

			shared_ptr<timeline::TimeLineEventDataList> m_eventDataList;
		public:
			virtual void Interpolation() = 0;
			template<class T>
			virtual void AddKeyFrame(const shared_ptr<void> data) = 0;
			virtual void ClearKeyFrame() = 0;

			virtual void Play() {}
			virtual void Reset() {}
			virtual void Stop() {}
		};
	}

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
