#pragma once
#include "stdafx.h"
#include "TimeLineEventDataList.h"
#include "../Utility/BaseSingleton.h"

namespace basecross {
	namespace timeline {
		class ClipBase;

		class TimeLine :public BaseSingletonGameObject<TimeLine> {
			float m_delta = 0.0f;
			bool m_isPlaying = false;
			map<wstring, shared_ptr<ClipBase>> m_clipList;
			shared_ptr<TimeLineEventDataList> m_eventList;
		public:
			TimeLine(const shared_ptr<Stage>& stage);
			void OnCreate()override;
			void OnUpdate()override;

			void Play();
			void Reset();

			void AddClip(const wstring& clipName, const shared_ptr<ClipBase>& clip);
			void AddEvent(float time, const function<void()>& func) { m_eventList->AddEvent(time, func); }

			template<class TClip>
			shared_ptr<TClip> CreateClip(const wstring& clipName) {
				auto clipData = make_shared<TClip>();
				m_clipList[clipName] = clipData;
				return clipData;
			}

			shared_ptr<ClipBase> GetClip(const wstring& clipName);

			template<class TClip>
			shared_ptr<TClip> GetClip(const wstring& clipName) {
				auto clipData = dynamic_pointer_cast<TClip>(GetClip(clipName));
				if (clipData) {
					return clipData;
				}

				wstring str(L"Clipデータを");
				str += Util::GetWSTypeName<T>();
				str += L"型にキャストできません";
				throw BaseException(
					str,
					L"!dynamic_pointer_cast<" + Util::GetWSTypeName<T>() + L">(GetClip(" + clipName + L"))",
					L"TimeLine::GetClip()"
				);
			}
		};
	}
}
