#include "stdafx.h"
#include "TimeLine.h"
#include "ClipBase.h"
#include "../DebugClass/Debug.h"

namespace basecross {
	// 静的メンバ変数の実体
	shared_ptr<timeline::TimeLine> timeline::TimeLine::m_ownInstance = nullptr;

	namespace timeline {
		TimeLine::TimeLine(const shared_ptr<Stage>& stage)
			:BaseSingletonGameObject(stage),
			m_eventList(make_shared<TimeLineEventDataList>())
		{}

		void TimeLine::OnCreate() {
			CreateInstance();
		}

		void TimeLine::OnUpdate() {
			if (!m_isPlaying)
				return;

			for (auto& clip : m_clipList) {
				clip->Update(m_delta);
			}
			m_eventList->Update(m_delta);

			m_delta += App::GetApp()->GetElapsedTime();
		}

		void TimeLine::AddClip(const shared_ptr<ClipBase>& clip) {
			m_clipList.push_back(clip);
		}

		void TimeLine::Play() {
			for (auto& clip : m_clipList) {
				clip->Initialize();
			}
			m_eventList->Initialize();

			m_isPlaying = true;
		}

		void TimeLine::Reset() {
			for (auto& clip : m_clipList) {
				clip->Reset();
			}
			m_delta = 0.0f;
			m_isPlaying = false;
		}
	}
}