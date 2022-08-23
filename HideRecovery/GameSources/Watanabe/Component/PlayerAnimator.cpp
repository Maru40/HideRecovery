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

	//--------------------------------------------------------------------------------------
	/// 遷移データ
	//--------------------------------------------------------------------------------------

	TransitionData::TransitionData() :
		TransitionData(nullptr, PlayerAnimationState::State(0))
	{}

	TransitionData::TransitionData(const std::function<bool()>& isTransition, const PlayerAnimationState::State& state) :
		isTransition(isTransition), transitionState(state)
	{}

	//--------------------------------------------------------------------------------------
	/// アニメーションイベント
	//--------------------------------------------------------------------------------------

	AnimationEvent::AnimationEvent() :
		AnimationEvent(nullptr, nullptr, nullptr)
	{}

	AnimationEvent::AnimationEvent(
		const std::function<void()>& start,
		const std::function<bool()>& update,
		const std::function<void()>& exit
	) :
		start(start),
		update(update),
		exit(exit)
	{}

	//--------------------------------------------------------------------------------------
	/// タイムアニメーションイベント
	//--------------------------------------------------------------------------------------

	TimeAnimationEvent::TimeAnimationEvent() :
		TimeAnimationEvent(0.0f, nullptr)
	{}

	TimeAnimationEvent::TimeAnimationEvent(const float time, const std::function<void()>& timeEvent) :
		time(time),
		timeEvent(timeEvent),
		isActive(true)
	{}

	//--------------------------------------------------------------------------------------
	/// プレイヤーアニメーター本体
	//--------------------------------------------------------------------------------------

	PlayerAnimator::PlayerAnimator(const shared_ptr<GameObject>& owner)
		:Animator(owner), m_beforeAnimationTime(0.0f)
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

		//自動でshotに遷移したいアニメーション
		{
			auto state = PlayerAnimationState::PlayerAnimationState2wstring(PlayerAnimationState::State::GunSet2);
			auto isTransition = [&]() { return IsTargetAnimationEnd(); };
			m_transitionDataMap[state] = TransitionData(isTransition, PlayerAnimationState::State::Shot);
		}
	}

	void PlayerAnimator::OnUpdate2() {
		Animator::OnUpdate();

		Transition();
		UpdateEvent();
		TimeEventUpdate();

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

		//デバッグ
		if (PlayerInputer::GetInstance()->IsUpDown()) {
			ChangePlayerAnimation(PlayerAnimationState::State::GunLeft);
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
		auto& events = m_animationEventsMap[currentAnimation];
		for (auto& event : events) {
			if (event.start) {
				event.start();
			}
		}
	}

	void PlayerAnimator::UpdateEvent() {
		auto draw = GetGameObject()->GetComponent<PNTBoneModelDraw>();
		auto currentAnimation = draw->GetCurrentAnimation();
		auto& events = m_animationEventsMap[currentAnimation];
		for (auto& event : events) {
			if (event.update) {
				event.update();
			}
		}
	}

	void PlayerAnimator::ExitEvent() {
		auto draw = GetGameObject()->GetComponent<PNTBoneModelDraw>();
		auto currentAnimation = draw->GetCurrentAnimation();
		auto& events = m_animationEventsMap[currentAnimation];
		for (auto& event : events) {
			if (event.exit) {
				event.exit();
			}
		}
	}

	void PlayerAnimator::TimeEventUpdate() {
		auto draw = GetGameObject()->GetComponent<PNTBoneModelDraw>();
		float currentTime = draw->GetCurrentAnimationTime();
		auto currentAnimation = draw->GetCurrentAnimation();
		auto& events = m_timeEventsMap[currentAnimation];
		for (auto& data : events) {
			if (!data.isActive) {	//アクティブでないなら処理をとばす
				continue;
			}

			if (data.time < currentTime) {	//時間を超えていたら。
				if (data.timeEvent) {
					data.timeEvent();
					data.isActive = false;
				}
			}
		}

		//アニメーションがループされたら
		if (currentTime < m_beforeAnimationTime) {
			for (auto& data : events) {
				if (data.isActive) {	//アクティブなら
					if (data.timeEvent) {
						data.timeEvent();
					}
				}

				data.isActive = true;
			}
		}

		m_beforeAnimationTime = currentTime;
	}

	void PlayerAnimator::ChangePlayerAnimation(PlayerAnimationState::State state) {
		ExitEvent();

		m_beforeAnimationTime = 0.0f;
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

	void PlayerAnimator::AddTimeEvent(const PlayerAnimationState::State& state, const float time, const std::function<void()>& timeEvent){
		AddTimeEvent(state, TimeAnimationEvent(time, timeEvent));
	}

	void PlayerAnimator::AddTimeEvent(const PlayerAnimationState::State& state, const TimeAnimationEvent& timeEvent){
		m_timeEventsMap[PlayerAnimationState::PlayerAnimationState2wstring(state)].push_back(timeEvent);
	}
}