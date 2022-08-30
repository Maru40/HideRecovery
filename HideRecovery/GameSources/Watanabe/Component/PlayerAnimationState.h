#pragma once
#include "stdafx.h"

namespace basecross {
	// MotionCSV2CEnumによる自動生成
	struct PlayerAnimationState {
		enum class State {
			Wait,
			Walk_R,
			Walk_L,
			Dstart,
			Dash,
			PutItem_Floor,
			PutItem_HideObject,
			GunSet1,
			GunSet2,
			GunFront,
			GunBack,
			GunRight,
			GunLeft,
			Shot,
			GunEnd1,
			GunEnd2,
			DAStart,
			DAttack,
			Dead,
			GSDead,
			Goal1,
			StartTeleport,
			ExcuteTeleport,
			EndTeleport,
			Win1 ,
			Win1Loop,
			Win2,
			Win2Loop,
			Win3,
			Win3Loop,
		};

		wstring static PlayerAnimationState2wstring(const PlayerAnimationState::State& state) {
			switch (state)
			{
			case PlayerAnimationState::State::Wait:
				return L"Wait";
			case PlayerAnimationState::State::Walk_R:
				return L"Walk_R";
			case PlayerAnimationState::State::Walk_L:
				return L"Walk_L";
			case PlayerAnimationState::State::Dstart:
				return L"Dstart";
			case PlayerAnimationState::State::Dash:
				return L"Dash";
			case PlayerAnimationState::State::PutItem_Floor:
				return L"PutItem_Floor";
			case PlayerAnimationState::State::PutItem_HideObject:
				return L"PutItem_HideObject";
			case PlayerAnimationState::State::GunSet1:
				return L"GunSet1";
			case PlayerAnimationState::State::GunSet2:
				return L"GunSet2";
			case PlayerAnimationState::State::GunFront:
				return L"GunFront";
			case PlayerAnimationState::State::GunBack:
				return L"GunBack";
			case PlayerAnimationState::State::GunRight:
				return L"GunRight";
			case PlayerAnimationState::State::GunLeft:
				return L"GunLeft";
			case PlayerAnimationState::State::Shot:
				return L"Shot";
			case PlayerAnimationState::State::GunEnd1:
				return L"GunEnd1";
			case PlayerAnimationState::State::GunEnd2:
				return L"GunEnd2";
			case PlayerAnimationState::State::DAStart:
				return L"DAStart";
			case PlayerAnimationState::State::DAttack:
				return L"DAttack";
			case PlayerAnimationState::State::Dead:
				return L"Dead";
			case PlayerAnimationState::State::GSDead:
				return L"GSDead";
			case PlayerAnimationState::State::Goal1:
				return L"Goal1";
			case PlayerAnimationState::State::StartTeleport:
				return L"StartTeleport";
			case PlayerAnimationState::State::ExcuteTeleport:
				return L"ExcuteTeleport";
			case PlayerAnimationState::State::EndTeleport:
				return L"EndTeleport";
			case PlayerAnimationState::State::Win1 :
				return L"Win1 ";
			case PlayerAnimationState::State::Win1Loop:
				return L"Win1Loop";
			case PlayerAnimationState::State::Win2:
				return L"Win2";
			case PlayerAnimationState::State::Win2Loop:
				return L"Win2Loop";
			case PlayerAnimationState::State::Win3:
				return L"Win3";
			case PlayerAnimationState::State::Win3Loop:
				return L"Win3Loop";

			default:
				break;
			}
			return L"";
		}
	};
}
