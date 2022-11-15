#pragma once
#include "stdafx.h"
#include "../Utility/BaseSingleton.h"

namespace basecross {
	namespace timeline {
		class ClipBase;
		class TimeLineEventDataList;

		class TimeLine :public BaseSingletonGameObject<TimeLine> {
			float m_delta = 0.0f;
			bool m_isPlaying = false;
			vector<shared_ptr<ClipBase>> m_clipList;

			shared_ptr<TimeLineEventDataList> m_eventList;
		public:
			TimeLine(const shared_ptr<Stage>& stage);
			void OnCreate()override;
			void OnUpdate()override;

			void AddClip(const shared_ptr<ClipBase>& clip);

			void Play();
			void Reset();
		};
	}
}
