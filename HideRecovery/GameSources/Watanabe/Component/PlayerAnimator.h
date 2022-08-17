#pragma once
#include "stdafx.h"
#include "Animator.h"
#include "PlayerAnimationState.h"

namespace basecross {
	class VelocityManager;

	class PlayerAnimator :public Animator {
		std::weak_ptr<VelocityManager> m_velocityManager;

	public:
		PlayerAnimator(const shared_ptr<GameObject>& owner);

		void OnCreate()override;
		void OnLateStart() override;
		void OnUpdate() override;

		void ChangePlayerAnimation(PlayerAnimationState::State state);

		/// <summary>
		/// 現在のステートを確認
		/// </summary>
		/// <param name="state">確認したいステート</param>
		bool IsCurretAnimationState(const PlayerAnimationState::State& state) const;

	};
}
