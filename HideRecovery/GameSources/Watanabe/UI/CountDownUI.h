#pragma once
#include "stdafx.h"
#include "Numbers.h"

namespace basecross {
	class CountDownUI :public GameObject {
		// 開始時間
		float m_startTime;
		// 現在の時間
		float m_presentTime;
		// カウントダウンが有効か
		bool m_isActive;
		shared_ptr<NumberSprite> m_number;
	public:
		CountDownUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
