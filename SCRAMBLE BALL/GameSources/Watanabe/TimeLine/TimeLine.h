#pragma once
#include "stdafx.h"
#include "KeyFrameData.h"
#include "AdvQueue.h"

namespace basecross {
	class TimeLine :public Component {
		bool m_isPlaying = false;
		float m_delta;
		weak_ptr<Camera> m_camera;

		shared_ptr<CameraKeyFrameData> m_currentKey;
		shared_ptr<CameraKeyFrameData> m_nextKey;
		vector<shared_ptr<CameraKeyFrameData>> m_keyFrameList;
		AdvQueue<shared_ptr<CameraKeyFrameData>> m_timeLine;
	public:
		TimeLine(const shared_ptr<GameObject>& owner);

		void OnCreate()override;
		void OnUpdate()override;
		void OnDraw()override {}

		void AddKeyFrame(const CameraKeyFrameData data);
		void Play();
		void Reset();
	};
}
