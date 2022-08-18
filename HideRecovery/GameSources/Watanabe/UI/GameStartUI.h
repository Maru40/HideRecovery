#pragma once
#include "stdafx.h"
#include "CountDownUI.h"
#include "SimpleSprite.h"

namespace basecross {
	class GameStartUI :public GameObject {
		// カウントダウンUI
		shared_ptr<CountDownUI> m_countDown;
		// 「Start」の文字表示用
		shared_ptr<SimpleSprite> m_strStart;
		bool m_isStart;
	public:
		GameStartUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override;
		void OnDestroy()override;

		void Start();
		bool IsGameBegan();
	};
}
