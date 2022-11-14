#pragma once
#include "stdafx.h"
#include "AdvQueue.h"
#include "KeyFrameData.h"
#include "TimeLineEventDataList.h"

namespace basecross {
	namespace timeline {
		class GameObjectTimeLine :public Component {
			bool m_isPlaying = false;
			float m_delta;
			weak_ptr<Transform> m_ownerTrans;

			shared_ptr<GameObjectKeyFrameData> m_currentKey;
			shared_ptr<GameObjectKeyFrameData> m_nextKey;
			vector<shared_ptr<GameObjectKeyFrameData>> m_keyFrameList;
			AdvQueue<shared_ptr<GameObjectKeyFrameData>> m_timeLine;

			shared_ptr<timeline::TimeLineEventDataList> m_eventDataList;
		public:
			GameObjectTimeLine(const shared_ptr<GameObject>& owner);

			void OnCreate()override;
			void OnUpdate()override;
			void OnDraw()override {}

			void AddKeyFrame(const GameObjectKeyFrameData data);
			void AddEvent(float time, const function<void()>& func);
			void Play();
			void Reset();
		};
	}
}
