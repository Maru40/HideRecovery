#pragma once
#include "stdafx.h"
#include "ClipBase.h"
#include "CameraKeyFrame.h"

namespace basecross {
	namespace timeline {
		class CameraClip :public ClipBase {
		public:
			CameraClip(const shared_ptr<GameObject>& owner);

			/// <summary>
			/// 補間する
			/// </summary>
			void Interpolation(
				const shared_ptr<CameraKeyFrame>& current,
				const shared_ptr<CameraKeyFrame>& next,
				float delta) {
				auto Eye = Lerp::CalculateLerp(current->Eye, next->Eye, current->Time, next->Time, delta, current->Rate);
				auto At = Lerp::CalculateLerp(current->At, next->At, current->Time, next->Time, delta, current->Rate);
			}
		};
	}
}
