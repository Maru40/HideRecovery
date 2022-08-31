#pragma once
#include "stdafx.h"

namespace basecross {
	// MotionCSV2CEnumによる自動生成
	struct BallAnimationState {
		enum class State {
			Goal,
			Gwait,
		};

		wstring static BallAnimationState2wstring(const BallAnimationState::State& state) {
			switch (state)
			{
			case BallAnimationState::State::Goal:
				return L"Goal";
			case BallAnimationState::State::Gwait:
				return L"Gwait";

			default:
				break;
			}
			return L"";
		}
	};
}
