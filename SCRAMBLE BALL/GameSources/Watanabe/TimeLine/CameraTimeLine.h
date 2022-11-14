﻿#pragma once
#include "stdafx.h"
#include "AdvQueue.h"
#include "KeyFrameData.h"
#include "../UI/UIObjectBase.h"
#include "TimeLineEventDataList.h"

namespace basecross {
	namespace timeline {
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
	}
}
