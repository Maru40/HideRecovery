#pragma once
#include "stdafx.h"
#include "../Interface/I_Performable.h"
#include "../UI/CountDownUI.h"
#include "../UI/SimpleSprite.h"
#include "../Utility/TimeCounter.h"

namespace basecross {
	class GameFinishUI :public GameObject, public I_Performable {
		// カウントダウンUI
		shared_ptr<CountDownUI> m_countDown;
		// 「Finish」の文字表示用
		shared_ptr<SimpleSprite> m_strFinish;
		bool m_isStart;
	public:
		GameFinishUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override;

		void Start()override;
		void Reset()override;

		bool IsFinish();
	};
}
