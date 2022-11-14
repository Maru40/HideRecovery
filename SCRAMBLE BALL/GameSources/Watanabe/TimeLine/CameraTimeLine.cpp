#include "stdafx.h"
#include "CameraTimeLine.h"

namespace basecross {
	namespace timeline {
		CameraTimeLine::CameraTimeLine(const shared_ptr<GameObject>& owner)
			:Component(owner), m_eventDataList(make_shared<timeline::TimeLineEventDataList>())
		{}

		void CameraTimeLine::OnCreate() {
			m_camera = GetStage()->GetView()->GetTargetCamera();
		}
		void CameraTimeLine::OnUpdate() {
			if (!m_isPlaying)
				return;

			if (m_delta > m_nextKey->Time) {
				m_currentKey = m_nextKey;
				if (!m_timeLine.empty()) {
					m_nextKey = m_timeLine.front();
					m_timeLine.pop();
				}
			}

			auto data = m_currentKey->Interpolation(m_nextKey, m_delta);

			m_camera.lock()->SetEye(data.Eye);
			m_camera.lock()->SetAt(data.At);

			m_eventDataList->Update(m_delta);

			m_delta += App::GetApp()->GetElapsedTime();
		}

		void CameraTimeLine::AddKeyFrame(const CameraKeyFrameData data) {
			m_timeLine.push(make_shared<CameraKeyFrameData>(data));
		}

		void CameraTimeLine::AddEvent(float time, const function<void()>& func) {
			m_eventDataList->AddEvent(time, func);
		}

		void CameraTimeLine::Play() {
			m_currentKey = m_timeLine.front();
			m_timeLine.pop();

			m_nextKey = m_timeLine.front();
			m_timeLine.pop();

			m_isPlaying = true;

			m_eventDataList->Initialize();
		}
		void CameraTimeLine::Reset() {
			m_delta = 0.0f;
		}

		// ----------------------------------------------------------------------
		// ----------------------------------------------------------------------
		// ----------------------------------------------------------------------

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

			m_delta += App::GetApp()->GetElapsedTime();
		}

		void UIObjectTimeLine::AddKeyFrame(const UIObjectKeyFrameData data) {
			m_timeLine.push(make_shared<UIObjectKeyFrameData>(data));
		}

		void UIObjectTimeLine::AddEvent(float time, const function<void()>& func) {
			m_eventDataList->AddEvent(time, func);
		}

		void UIObjectTimeLine::Play() {
			m_currentKey = m_timeLine.front();
			m_timeLine.pop();

			m_nextKey = m_timeLine.front();
			m_timeLine.pop();

			m_isPlaying = true;

			m_eventDataList->Initialize();
		}
		void UIObjectTimeLine::Reset() {
			m_delta = 0.0f;
		}
	}
}