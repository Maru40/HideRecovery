#include "stdafx.h"
#include "UIObjectTimeLine.h"

namespace basecross {
	namespace timeline {
		UIObjectTimeLine::UIObjectTimeLine(const shared_ptr<GameObject>& owner)
			:Component(owner), m_eventDataList(make_shared<timeline::TimeLineEventDataList>())
		{}

		void UIObjectTimeLine::OnCreate() {
			m_ownerRect = GetGameObject()->GetComponent<RectTransform>();
		}
		void UIObjectTimeLine::OnUpdate() {
			if (!m_isPlaying)
				return;

			if (m_delta > m_nextKey->Time) {
				m_currentKey = m_nextKey;
				if (m_timeLine.empty()) {
					return;
				}
				m_nextKey = m_timeLine.front();
				m_timeLine.pop();
			}

			auto data = m_currentKey->Interpolation(m_nextKey, m_delta);

			auto rectTrans = m_ownerRect.lock();
			rectTrans->SetPosition(data.Position);
			rectTrans->SetScale(data.Scale);
			rectTrans->SetRotation(data.Rotation);

			m_eventDataList->Update(m_delta);
		}

		void UIObjectTimeLine::OnUpdate2() {
			m_delta += App::GetApp()->GetElapsedTime();
		}

		void UIObjectTimeLine::AddKeyFrame(const UIObjectKeyFrameData data) {
			m_timeLine.push(make_shared<UIObjectKeyFrameData>(data));
		}

		void UIObjectTimeLine::AddEvent(float time, const function<void()>& func) {
			m_eventDataList->AddEvent(time, func);
		}

		void UIObjectTimeLine::Play() {
			if (!m_timeLine.empty()) {
				m_currentKey = m_timeLine.front();
				m_timeLine.pop();

				m_nextKey = m_timeLine.front();
				m_timeLine.pop();
			}

			m_isPlaying = true;

			m_eventDataList->Initialize();
		}
		void UIObjectTimeLine::Reset() {
			m_delta = 0.0f;
		}
	}
}