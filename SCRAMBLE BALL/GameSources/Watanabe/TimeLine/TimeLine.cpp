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
				clip.second->Update(m_delta);
			}
			m_eventList->Update(m_delta);

			m_delta += App::GetApp()->GetElapsedTime();
		}

		void TimeLine::AddClip(const wstring& clipName, const shared_ptr<ClipBase>& clip) {
			m_clipList[clipName] = clip;
		}

		shared_ptr<ClipBase> TimeLine::GetClip(const wstring& clipName) {
			if (m_clipList.count(clipName) == 0) {
				throw BaseException(
					L"指定したデータがありません",
					clipName + L" is Undifined",
					L"TimeLine::GetClip()"
				);
			}

			return m_clipList[clipName];
		}

		void TimeLine::Play() {
			for (auto& clip : m_clipList) {
				clip.second->Initialize();
			}
			m_eventList->Initialize();

			m_isPlaying = true;
		}

		void TimeLine::Reset() {
			for (auto& clip : m_clipList) {
				clip.second->Reset();
			}
			m_delta = 0.0f;
			m_isPlaying = false;
		}
	}
}