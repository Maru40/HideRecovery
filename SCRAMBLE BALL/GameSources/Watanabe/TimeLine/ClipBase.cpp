#include "stdafx.h"
#include "ClipBase.h"

namespace basecross {
	namespace timeline {
		ClipBase::ClipBase(const shared_ptr<GameObject>& owner)
			:Component(owner)
		{}

		void ClipBase::Update(float delta) {
			if (m_keyFrameQueue.empty())
				return;

			auto nextKeyFrame = m_keyFrameQueue.front();
			if (delta > nextKeyFrame->Time) {
				m_currentKeyFrame = nextKeyFrame;
				if (!m_keyFrameQueue.empty()) {
					m_keyFrameQueue.pop();
				}
			}

			// 補完処理
			auto data = Interpolation(m_currentKeyFrame, nextKeyFrame, delta);
			// データの適用
			ApplyDataToObject(data);
		}

		void ClipBase::Initialize() {
			// 実行順にソート
			m_keyFrameList.sort(
				[](shared_ptr<KeyFrameBase> left, shared_ptr<KeyFrameBase> right) {
					return left->Time < right->Time;
				}
			);
			m_keyFrameQueue = AdvQueue<shared_ptr<KeyFrameBase>>(m_keyFrameList);

			m_currentKeyFrame = m_keyFrameQueue.front();
			m_keyFrameQueue.pop();
		}

		void ClipBase::Reset() {
		}

		void ClipBase::AddKeyFrame(const shared_ptr<KeyFrameBase>& keyFrame) {
			m_keyFrameList.push_back(keyFrame);
		}
	}
}