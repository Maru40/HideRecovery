#pragma once
#include "stdafx.h"
#include "CountDownUI.h"
#include "SimpleSprite.h"
#include "../Utility/TimeCounter.h"
#include "../Interface/I_Performable.h"

namespace basecross {
	class GameStartUI :public GameObject, public I_Performable {
		// カウントダウンUI
		shared_ptr<CountDownUI> m_countDown;
		// 「Start」の文字表示用
		shared_ptr<SimpleSprite> m_strStart;
		// 消えるまでのタイマー
		TimeCounter m_invisibleTimer;
		bool m_isStart;
	public:
		GameStartUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override;

		void Start()override;
		void Reset()override;
		bool IsGameBegin();
	};
}
