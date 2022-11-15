#pragma once
#include "stdafx.h"
#include "../Utility/TransformData.h"

namespace basecross {
	namespace timeline {
		struct CameraKeyFrame {
			Vec3 Eye;
			Vec3 At;
			float Time;			// キーフレーム位置
			Lerp::rate Rate;	// このフレームからの保管方法

			CameraKeyFrame(const Vec3& eye, const Vec3& at, float time, const Lerp::rate& rate)
				:Eye(eye), At(at), Time(time), Rate(rate)
			{}

			// ソート用
			bool operator<(const CameraKeyFrame& right) const {
				return Time < right.Time;
			}
		};
	}
}
