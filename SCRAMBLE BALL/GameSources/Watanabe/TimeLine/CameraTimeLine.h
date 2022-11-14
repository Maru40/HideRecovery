#pragma once
#include "stdafx.h"
#include "AdvQueue.h"
#include "KeyFrameData.h"
#include "../UI/UIObjectBase.h"
#include "TimeLineEventDataList.h"

namespace basecross {
	namespace timeline {
		class TimeLine {
			bool m_isPlaying = false;
			float m_delta;
		public:
			virtual void Interpolation() = 0;
			virtual void AddKeyFrame(const CameraKeyFrameData data) = 0;
			virtual void ClearKeyFrame() = 0;

			virtual void Play() {}
			virtual void Reset() {}
			virtual void Stop() {}
		};

		class CameraTimeLine :public Component {
			bool m_isPlaying = false;
			float m_delta;
			weak_ptr<Camera> m_camera;

			shared_ptr<CameraKeyFrameData> m_currentKey;
			shared_ptr<CameraKeyFrameData> m_nextKey;
			vector<shared_ptr<CameraKeyFrameData>> m_keyFrameList;
			AdvQueue<shared_ptr<CameraKeyFrameData>> m_timeLine;

			shared_ptr<timeline::TimeLineEventDataList> m_eventDataList;
		public:
			CameraTimeLine(const shared_ptr<GameObject>& owner);

			void OnCreate()override;
			void OnUpdate()override;
			void OnUpdate2()override;
			void OnDraw()override {}

			void AddKeyFrame(const CameraKeyFrameData data);
			void AddEvent(float time, const function<void()>& func);
			void Play();
			void Reset();
		};

		class UIObjectTimeLine :public Component {
			bool m_isPlaying = false;
			float m_delta;
			weak_ptr<RectTransform> m_ownerRect;

			shared_ptr<UIObjectKeyFrameData> m_currentKey;
			shared_ptr<UIObjectKeyFrameData> m_nextKey;
			vector<shared_ptr<UIObjectKeyFrameData>> m_keyFrameList;
			AdvQueue<shared_ptr<UIObjectKeyFrameData>> m_timeLine;

			shared_ptr<timeline::TimeLineEventDataList> m_eventDataList;
		public:
			UIObjectTimeLine(const shared_ptr<GameObject>& owner);

			void OnCreate()override;
			void OnUpdate()override;
			void OnUpdate2()override;
			void OnDraw()override {}

			void AddKeyFrame(const UIObjectKeyFrameData data);
			void AddEvent(float time, const function<void()>& func);
			void Play();
			void Reset();
		};
	}
}
