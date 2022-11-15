#include "stdafx.h"
#include "ClipBase.h"

namespace basecross {
	namespace timeline {
		ClipBase::ClipBase()
		{}

		void ClipBase::Update(float delta) {
			if (m_keyFrameQueue.empty())
				return;

			// 現在のキーフレームを通過
			if (delta > m_currentKeyFrame->Time) {
				auto nextKeyFrame = m_keyFrameQueue.front();
				// 次のキーフレームを通過
				if (delta > nextKeyFrame->Time) {
					// キーフレームを前に進める
					m_currentKeyFrame = nextKeyFrame;
					// 空でなければ1つ削除
					if (!m_keyFrameQueue.empty()) {
						m_keyFrameQueue.pop();
					}
					else {
						// キューが空＝次のキーフレームが無い
						return;
					}
				}

				// 補完処理
				auto data = Interpolation(m_currentKeyFrame, nextKeyFrame, delta);
				// データの適用
				ApplyDataToObject(data);
			}
		}

		void ClipBase::Initialize() {
			// リストが空なら何もしない
			if (m_keyFrameList.empty())
				return;

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