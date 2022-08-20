#pragma once
#include "stdafx.h"
#include "Numbers.h"

namespace basecross {
	class CountDownUI :public GameObject {
		// 開始時間
		float m_startTime;
		// 現在の時間
		float m_presentTime;
		// カウントダウンが終わったか
		bool m_isTimeUp;
		shared_ptr<NumberSprite> m_number;
		// 終了時のイベント
		function<void()> m_endTimeEvent;
	public:
		CountDownUI(const shared_ptr<Stage>& stage);
		CountDownUI(const shared_ptr<Stage>& stage, float countTime);

		void OnCreate()override;
		void OnUpdate()override;

		void Start();
		void Reset();
		bool IsTimeUp();

		void SetScale(float size);
		void SetColor(Col4 color);
	};
}
