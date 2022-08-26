#pragma once
#include "stdafx.h"

namespace basecross {
	// MotionCSV2CEnumによる自動生成
	struct BoxAnimationState {
		enum class State {
			Open,
			Wait,
			Close,
		};

		wstring static BoxAnimationState2wstring(const BoxAnimationState::State& state) {
			switch (state)
			{
			case BoxAnimationState::State::Open:
				return L"Open";
			case BoxAnimationState::State::Wait:
				return L"Wait";
			case BoxAnimationState::State::Close:
				return L"Close";

			default:
				break;
			}
			return L"";
		}
	};
}
