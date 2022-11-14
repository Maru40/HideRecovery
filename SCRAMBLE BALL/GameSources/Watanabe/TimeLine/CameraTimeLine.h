#pragma once
#include "stdafx.h"
#include "AdvQueue.h"
#include "KeyFrameData.h"

namespace basecross {
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

	// 1つのオブジェクトに対するタイムライン
	class CameraTimeLine : public TimeLine {
		weak_ptr<Camera> m_camera;

		shared_ptr<CameraKeyFrameData> m_currentKey;
		shared_ptr<CameraKeyFrameData> m_nextKey;
		vector<shared_ptr<CameraKeyFrameData>> m_keyFrameList;
		AdvQueue<shared_ptr<CameraKeyFrameData>> m_timeLine;
	public:
		void Interpolation()override;
		void AddKeyFrame(const CameraKeyFrameData data)override;
		void ClearKeyFrame()override;
	};

	class GameObjectTimeLine {
	};
}
