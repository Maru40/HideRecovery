#pragma once
#include "stdafx.h"
#include "KeyFrameData.h"
#include "AdvQueue.h"

namespace basecross {
	namespace timeline {
		class ClipBase :public Component {
		public:
			ClipBase(const shared_ptr<GameObject>& owner);

			void OnCreate()override;
			void OnUpdate()override {}
			void OnDraw()override {}

			void Update(float delta);
			void AddKeyFrame();
		};
	}
}
