/*!
@file   PlayerAnimator.h
@brief  プレイヤーのアニメーションクラス
*/

#pragma once
#include "stdafx.h"
#include "Animator.h"
#include "PlayerAnimationState.h"

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

	class PlayerAnimator :public Animator {
		std::weak_ptr<VelocityManager> m_velocityManager;

		std::unordered_map<wstring, TransitionData> m_transitionDataMap;
		std::unordered_map<wstring, std::vector<AnimationEvent>> m_animationEventsMap;
		
	public:
		PlayerAnimator(const shared_ptr<GameObject>& owner);
		void OnCreate()override;
		void OnLateStart() override;
		void OnUpdate() override;

	private:
		void Transition();

		void StartEvent();
		void UpdateEvent();
		void ExitEvent();

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

	};
}
