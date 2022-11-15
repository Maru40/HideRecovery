#pragma once
#include "stdafx.h"
#include "KeyFrameData.h"
#include "AdvQueue.h"

namespace basecross {
	namespace timeline {
		class ClipBase :public Component {
			shared_ptr<KeyFrameBase> m_currentKeyFrame;
			shared_ptr<KeyFrameBase> m_nextKeyFrame;

			list<shared_ptr<KeyFrameBase>> m_keyFrameList;
			AdvQueue<shared_ptr<KeyFrameBase>> m_keyFrameQueue;
		public:
			ClipBase(const shared_ptr<GameObject>& owner);

			virtual void OnCreate()override {}
			void OnUpdate()override {}
			void OnDraw()override {}

			void Initialize();
			void Update(float delta);
			void Reset();

			/// <summary>
			/// 補完処理
			/// </summary>
			/// <param name="current"></param>
			/// <param name="next"></param>
			/// <param name="delta"></param>
			/// <returns></returns>
			virtual shared_ptr<void> Interpolation(
				const shared_ptr<KeyFrameBase>& current,
				const shared_ptr<KeyFrameBase>& next,
				float delta) = 0;

			/// <summary>
			/// 補間したデータをオブジェクトに適用する
			/// </summary>
			/// <param name="objectAnimationData"></param>
			virtual void ApplyDataToObject(const shared_ptr<void>& objectAnimationData) = 0;

			void AddKeyFrame(const shared_ptr<KeyFrameBase>& keyFrame);
		};
	}
}
