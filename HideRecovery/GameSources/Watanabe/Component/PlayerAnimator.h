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

	class PlayerAnimator :public Animator {
		std::weak_ptr<VelocityManager> m_velocityManager;

		std::unordered_map<wstring, TransitionData> m_transitionDataMap;

	public:
		PlayerAnimator(const shared_ptr<GameObject>& owner);
		void OnCreate()override;
		void OnLateStart() override;
		void OnUpdate2() override;

	private:
		void Transition();

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

	};
}
