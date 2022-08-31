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
	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class VelocityManager;

	//--------------------------------------------------------------------------------------
	/// 遷移データ
	//--------------------------------------------------------------------------------------
	struct TransitionData {
		std::function<bool()> isTransition;				//遷移条件
		PlayerAnimationState::State transitionState;	//遷移したいステート

		TransitionData();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="isTransition">遷移できる状態かどうか</param>
		/// <param name="state">ステート</param>
		TransitionData(const std::function<bool()>& isTransition, const PlayerAnimationState::State& state);
	};

	//--------------------------------------------------------------------------------------
	/// アニメーションイベント
	//--------------------------------------------------------------------------------------
	struct AnimationEvent {
		std::function<void()> start;	//開始イベント
		std::function<bool()> update;	//更新イベント
		std::function<void()> exit;		//終了イベント

		AnimationEvent();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="start">開始イベント</param>
		/// <param name="update">更新イベント</param>
		/// <param name="exit">終了イベント</param>
		AnimationEvent(
			const std::function<void()>& start,
			const std::function<bool()>& update,
			const std::function<void()>& exit
		);
	};

	//--------------------------------------------------------------------------------------
	/// タイムアニメーションイベント
	//--------------------------------------------------------------------------------------
	struct TimeAnimationEvent {
		float time;		//時間
		std::function<void()> timeEvent;	//タイムイベント
		bool isActive;	//アクティブ

		TimeAnimationEvent();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="time">タイム</param>
		/// <param name="timeEvent">タイムイベント</param>
		TimeAnimationEvent(const float time, const std::function<void()>& timeEvent);
	};

	//--------------------------------------------------------------------------------------
	/// プレイヤーアニメーター
	//--------------------------------------------------------------------------------------
	class PlayerAnimator : public Animator
	{
		float m_waitTransitionSpeed;
		float m_walkTransitionSpeed;

		std::weak_ptr<VelocityManager> m_velocityManager;								//速度管理

		std::unordered_map<wstring, std::vector<TransitionData>> m_transitionDatasMap;	//遷移データマップ
		std::unordered_map<wstring, std::vector<AnimationEvent>> m_animationEventsMap;	//アニメーションイベントマップ
		std::unordered_map<wstring, std::vector<TimeAnimationEvent>> m_timeEventsMap;	//タイムイベントマップ

		float m_beforeAnimationTime;	//前フレームアニメーションタイム管理
		
	public:
		PlayerAnimator(const shared_ptr<GameObject>& owner);

		void OnCreate() override;
		void OnLateStart() override;
		void OnUpdate2() override;

	private:
		/// <summary>
		/// 遷移管理
		/// </summary>
		void Transition();

		void StartEvent();
		void UpdateEvent();
		void ExitEvent();

		/// <summary>
		/// タイムイベント更新
		/// </summary>
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
