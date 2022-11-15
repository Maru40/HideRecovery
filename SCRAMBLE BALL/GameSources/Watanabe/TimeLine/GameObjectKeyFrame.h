#pragma once
#include "stdafx.h"
#include "KeyFrameData.h"
#include "../Utility/TransformData.h"

namespace basecross {
	namespace timeline {
		struct GameObjectKeyFrame :public KeyFrameBase {
			TransformData TransData;

			GameObjectKeyFrame(const TransformData& data, float time, const Lerp::rate& rate)
				:TransData(data), KeyFrameBase(time, rate)
			{}

			static shared_ptr<GameObjectKeyFrame> Create(const TransformData& data, float time, const Lerp::rate& rate) {
				return make_shared<GameObjectKeyFrame>(data, time, rate);
			}
		};
	}
}
