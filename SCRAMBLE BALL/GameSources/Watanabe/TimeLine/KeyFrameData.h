#pragma once
#include "stdafx.h"
#include "../Utility/TransformData.h"

namespace basecross {
	namespace timeline {
		struct KeyFrameBase {
			float Time;			// キーフレーム位置
			Lerp::rate Rate;	// このフレームからの保管方法

			KeyFrameBase(float time, const Lerp::rate& rate)
				:Time(time), Rate(rate)
			{}

			// ソート用
			bool operator<(const KeyFrameBase& right) const {
				return Time < right.Time;
			}
		};
	}
}
