/*!
@file   PlayerAnimator.h
@brief  プレイヤーのアニメーションクラス
*/

#pragma once
#include "stdafx.h"
#include "Animator.h"
#include "PlayerAnimationState.h"

#include "MaruAction.h"

namespace basecross {
	class VelocityManager;

	struct TransitionData {
		std::function<bool()> isTransition;				//遷移条件
		PlayerAnimationState::State transitionState;	//遷移したいステート

		TransitionData() :
			TransitionData(nullptr, PlayerAnimationState::State(0))
		{}

		TransitionData(const std::function<bool()>& isTransition, const PlayerAnimationState::State& state) :
			isTransition(isTransition), transitionState(state)
		{}
	};

	struct AnimationEvent {
		std::function<void()> start;
		std::function<bool()> update;
		std::function<void()> exit;

		AnimationEvent():
			AnimationEvent(nullptr, nullptr, nullptr)
		{}

		AnimationEvent(
			const std::function<void()>& start, 
			const std::function<bool()>& update,
			const std::function<void()>& exit
		) :
			start(start),
			update(update),
			exit(exit)
		{}
	};

	struct TimeAnimationEvent {
		float time;
		std::function<void()> timeEvent;
		bool isActive;

		TimeAnimationEvent();

		TimeAnimationEvent(const float time, const std::function<void()>& timeEvent);
	};

	class PlayerAnimator :public Animator {
		std::weak_ptr<VelocityManager> m_velocityManager;

		std::unordered_map<wstring, TransitionData> m_transitionDataMap;
		std::unordered_map<wstring, std::vector<AnimationEvent>> m_animationEventsMap;
		std::unordered_map<wstring, std::vector<TimeAnimationEvent>> m_timeEventsMap;

		float m_beforeAnimationTime;
		
	public:
		PlayerAnimator(const shared_ptr<GameObject>& owner);
		void OnCreate()override;
		void OnLateStart() override;
		void OnUpdate2() override;

	private:
		void Transition();

		void StartEvent();
		void UpdateEvent();
		void ExitEvent();

		void TimeEventUpdate();

	public:
		/// <summary>
		/// アニメーションの変更
		/// </summary>
		/// <param name="state">変更するアニメーションのステート</param>
		void ChangePlayerAnimation(PlayerAnimationState::State state);

		/// <summary>
		/// 現在のステートを確認
		/// </summary>
		/// <param name="state">確認したいステート</param>
		bool IsCurretAnimationState(const PlayerAnimationState::State& state) const;

		/// <summary>
		/// アニメーションイベントの設定
		/// </summary>
		/// <param name="state">ステート</param>
		/// <param name="start">開始時イベント</param>
		/// <param name="update">更新イベント</param>
		/// <param name="exit">終了時イベント</param>
		void AddAnimationEvent(
			const PlayerAnimationState::State& state,
			const std::function<void()>& start,
			const std::function<bool()>& update,
			const std::function<void()>& exit
		);

		/// <summary>
		/// アニメーションイベントの設定
		/// </summary>
		/// <param name="state">ステート</param>
		/// <param name="animationEvent">アニメーションイベント</param>
		void AddAnimationEvent(const PlayerAnimationState::State& state, const AnimationEvent& animationEvent);

		/// <summary>
		/// タイムイベントの追加
		/// </summary>
		/// <param name="state">ステート</param>
		/// <param name="time">時間</param>
		/// <param name="function">イベント</param>
		void AddTimeEvent(const PlayerAnimationState::State& state, const float time, const std::function<void()>& timeEvent);

		/// <summary>
		/// タイムイベントの追加
		/// </summary>
		/// <param name="state">ステート</param>
		/// <param name="timeEvent">タイムイベント</param>
		void AddTimeEvent(const PlayerAnimationState::State& state, const TimeAnimationEvent& timeEvent);
	};
}
