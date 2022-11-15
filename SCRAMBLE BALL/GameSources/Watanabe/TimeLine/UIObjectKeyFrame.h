#pragma once
#include "stdafx.h"
#include "KeyFrameBase.h"
#include "../Utility/TransformData.h"

namespace basecross {
	namespace timeline {
		struct UIObjectKeyFrame :public KeyFrameBase {
			RectTransformData RectData;

			UIObjectKeyFrame(const RectTransformData& data, float time, const Lerp::rate& rate)
				:RectData(data), KeyFrameBase(time, rate)
			{}

			static shared_ptr<UIObjectKeyFrame> Create(const RectTransformData& data, float time, const Lerp::rate& rate) {
				return make_shared<UIObjectKeyFrame>(data, time, rate);
			}
		};
	}
}
