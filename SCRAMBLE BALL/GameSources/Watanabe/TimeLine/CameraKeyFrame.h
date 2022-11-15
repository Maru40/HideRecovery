#pragma once
#include "stdafx.h"
#include "KeyFrameData.h"

namespace basecross {
	namespace timeline {
		struct CameraKeyFrame :public KeyFrameBase {
			Vec3 Eye;
			Vec3 At;

			CameraKeyFrame(const Vec3& eye, const Vec3& at, float time, const Lerp::rate& rate)
				:Eye(eye), At(at), KeyFrameBase(time, rate)
			{}
		};
	}
}
