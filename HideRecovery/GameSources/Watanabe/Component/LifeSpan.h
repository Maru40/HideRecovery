/*!
@file   LifeSpan.h
@brief  寿命を持たせるためのコンポーネント
*/

#pragma once
#include "stdafx.h"
#include "../Utility/TimeCounter.h"

namespace basecross {
	class LifeSpan :public Component {
		TimeCounter m_timer;
	public:
		LifeSpan(const shared_ptr<GameObject>& gameObjectPtr, float interval = 1.0f)
			:Component(gameObjectPtr), m_timer(interval)
		{}

		void OnCreate()override {}
		void OnUpdate()override;
		void OnDraw()override {}

		void SetLifeTime(float lifeTime) {
			m_timer.SetIntervalTime(lifeTime);
		}
	};
}
