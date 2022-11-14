#include "stdafx.h"
#include "GameObjectTimeLine.h"

namespace basecross {
	namespace timeline {
		GameObjectTimeLine::GameObjectTimeLine(const shared_ptr<GameObject>& owner)
			:Component(owner), m_eventDataList(make_shared<timeline::TimeLineEventDataList>())
		{}

		void GameObjectTimeLine::OnCreate() {
			m_ownerTrans = GetGameObject()->GetComponent<Transform>();
		}
		void GameObjectTimeLine::OnUpdate() {
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

			auto rectTrans = m_ownerTrans.lock();
			rectTrans->SetPosition(data.Position);
			rectTrans->SetScale(data.Scale);
			rectTrans->SetRotation(data.Rotation);

			m_eventDataList->Update(m_delta);

			m_delta += App::GetApp()->GetElapsedTime();
		}

		void GameObjectTimeLine::AddKeyFrame(const GameObjectKeyFrameData data) {
			m_timeLine.push(make_shared<GameObjectKeyFrameData>(data));
		}

		void GameObjectTimeLine::AddEvent(float time, const function<void()>& func) {
			m_eventDataList->AddEvent(time, func);
		}

		void GameObjectTimeLine::Play() {
			if (!m_timeLine.empty()) {
				m_currentKey = m_timeLine.front();
				m_timeLine.pop();

				m_nextKey = m_timeLine.front();
				m_timeLine.pop();
			}

			m_isPlaying = true;

			m_eventDataList->Initialize();
		}
		void GameObjectTimeLine::Reset() {
			m_delta = 0.0f;
		}
	}
}