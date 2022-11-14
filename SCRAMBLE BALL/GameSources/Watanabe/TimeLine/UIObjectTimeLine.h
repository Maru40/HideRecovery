#pragma once
#include "stdafx.h"
#include "AdvQueue.h"
#include "KeyFrameData.h"
#include "../UI/UIObjectBase.h"
#include "TimeLineEventDataList.h"

namespace basecross {
	namespace timeline {
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
