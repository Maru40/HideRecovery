#pragma once
#include "stdafx.h"
#include "AdvQueue.h"
#include "KeyFrameData.h"

namespace basecross {
	class TimeLine {
	public:
		virtual void Interpolation() = 0;
	};

	// 1つのオブジェクトに対するタイムライン
	class CameraTimeLine : public TimeLine {
		bool m_isPlaying = false;
		float m_delta;
		weak_ptr<Camera> m_camera;

		shared_ptr<CameraKeyFrameData> m_currentKey;
		shared_ptr<CameraKeyFrameData> m_nextKey;
		vector<shared_ptr<CameraKeyFrameData>> m_keyFrameList;
		AdvQueue<shared_ptr<CameraKeyFrameData>> m_timeLine;
	public:
		void AddKeyFrame(const CameraKeyFrameData data);
		void ClearKeyFrame();

		void Play();
		void Reset();
		void Stop();
	};

	class GameObjectTimeLine {
	};
}
