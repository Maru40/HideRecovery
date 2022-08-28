﻿#pragma once
#include "stdafx.h"
#include "Animator.h"
#include "BoxAnimationState.h"

namespace basecross {
	class BoxAnimator :public Animator 
	{
	public:
		BoxAnimator(const shared_ptr<GameObject>& owner);

		void OnCreate() override;

		/// <summary>
		/// アニメーションの変更
		/// </summary>
		/// <param name="state">変更するアニメーションのステート</param>
		void ChangeBoxAnimation(BoxAnimationState::State state);
	};
}
