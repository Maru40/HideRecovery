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
		bool m_isRemove;
	public:
		LifeSpan(const shared_ptr<GameObject>& gameObjectPtr)
			:Component(gameObjectPtr), m_timer(1.0f), m_isRemove(true)
		{}

		void OnCreate()override {}
		void OnUpdate()override;
		void OnDraw()override {}

		void SetLifeTime(float lifeTime) {
			m_timer.SetIntervalTime(lifeTime);
		}

		void IsRemove(bool flg) {
			m_isRemove = flg;
		}

		void Reset() {
			m_timer.Reset();
		}
	};
}
