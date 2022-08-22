/*!
@file   PlayerAnimator.cpp
@brief  プレイヤーのアニメーションクラス実体
*/

#include "stdafx.h"
#include "PlayerAnimator.h"

#include "VelocityManager.h"

#include "PlayerInputer.h"
#include "Maruyama/Player/Component/GoalAnimationController.h"
#include "Maruyama/StageObject/Goal.h"
#include "MaruUtility.h"

namespace basecross {
	PlayerAnimator::PlayerAnimator(const shared_ptr<GameObject>& owner)
		:Animator(owner)
	{}

	void PlayerAnimator::OnCreate() {
		auto data = LoadAnimationData(L"PlayerAnimation");
		for (auto d : data) {
			RegisterAnimationClip(d);
		}
	}

	void PlayerAnimator::OnLateStart() {
		m_velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);

		//自動でwaitに変更したいアニメーション一覧
		PlayerAnimationState::State baseStates[] = {
			PlayerAnimationState::State::PutItem_Floor,
			PlayerAnimationState::State::PutItem_HideObject,
			PlayerAnimationState::State::GunEnd2
		};

		for (auto& baseState : baseStates) {
			auto state = PlayerAnimationState::PlayerAnimationState2wstring(baseState);
			auto isTransition = [&]() { return IsTargetAnimationEnd(); };
			m_transitionDataMap[state] = TransitionData(isTransition, PlayerAnimationState::State::Wait);
		}
	}

	void PlayerAnimator::OnUpdate2() {
		Animator::OnUpdate();

		Transition();
		UpdateEvent();

		auto velocityManager = m_velocityManager.lock();
		if (!velocityManager) {
			return;
		}

		constexpr float TransitionSpeed = 0.1f;	//遷移速度
		auto velocity = velocityManager->GetVelocity();
		velocity.y = 0.0f;

		//速度が一定以下かつ、Dash状態なら
		if (velocity.length() < TransitionSpeed && IsCurretAnimationState(PlayerAnimationState::State::Dash)) {
			ChangePlayerAnimation(PlayerAnimationState::State::Wait);
		}

		//速度が一定より大きいかつ、Wait状態なら
		if (velocity.length() > TransitionSpeed && IsCurretAnimationState(PlayerAnimationState::State::Wait)) {
			ChangePlayerAnimation(PlayerAnimationState::State::Dash);
		}

		return;
		//デバッグ
		if (PlayerInputer::GetInstance()->IsUpDown()) {
			if (auto goalCtrl = GetGameObject()->GetComponent<GoalAnimationController>(false)) {
				goalCtrl->SetDunkPosition(maru::Utility::FindComponent<Goal>()->GetDunkPosition());
			}

			ChangePlayerAnimation(PlayerAnimationState::State::Goal1);
		}
	}

	void PlayerAnimator::Transition() {
		auto draw = GetGameObject()->GetComponent<PNTBoneModelDraw>();
		auto currentAnimation = draw->GetCurrentAnimation();

		if (m_transitionDataMap.count(currentAnimation) == 0) {	//メンバーがないなら処理をしない。
			return;
		}

		auto& data = m_transitionDataMap.at(currentAnimation);

		if (data.isTransition()) {
			ChangePlayerAnimation(data.transitionState);
		}
	}

	void PlayerAnimator::StartEvent() {
		auto draw = GetGameObject()->GetComponent<PNTBoneModelDraw>();
		auto currentAnimation = draw->GetCurrentAnimation();
		auto events = m_animationEventsMap[currentAnimation];
		for (auto& event : events) {
			if (event.start) {
				event.start();
			}
		}
	}

	void PlayerAnimator::UpdateEvent() {
		auto draw = GetGameObject()->GetComponent<PNTBoneModelDraw>();
		auto currentAnimation = draw->GetCurrentAnimation();
		auto events = m_animationEventsMap[currentAnimation];
		for (auto& event : events) {
			if (event.update) {
				event.update();
			}
		}
	}

	void PlayerAnimator::ExitEvent() {
		auto draw = GetGameObject()->GetComponent<PNTBoneModelDraw>();
		auto currentAnimation = draw->GetCurrentAnimation();
		auto events = m_animationEventsMap[currentAnimation];
		for (auto& event : events) {
			if (event.exit) {
				event.exit();
			}
		}
	}

	void PlayerAnimator::ChangePlayerAnimation(PlayerAnimationState::State state) {
		ExitEvent();

		ChangeAnimation(PlayerAnimationState::PlayerAnimationState2wstring(state));

		StartEvent();
	}

	bool PlayerAnimator::IsCurretAnimationState(const PlayerAnimationState::State& state) const {
		auto drawer = GetGameObject()->GetComponent<PNTBoneModelDraw>();
		auto strState = PlayerAnimationState::PlayerAnimationState2wstring(state);

		return strState == drawer->GetCurrentAnimation();
	}

	void PlayerAnimator::AddAnimationEvent(
		const PlayerAnimationState::State& state,
		const std::function<void()>& start,
		const std::function<bool()>& update,
		const std::function<void()>& exit
	)
	{
		AddAnimationEvent(state, AnimationEvent(start, update, exit));
	}

	void PlayerAnimator::AddAnimationEvent(const PlayerAnimationState::State& state, const AnimationEvent& animationEvent) {
		m_animationEventsMap[PlayerAnimationState::PlayerAnimationState2wstring(state)].push_back(animationEvent);
	}
}