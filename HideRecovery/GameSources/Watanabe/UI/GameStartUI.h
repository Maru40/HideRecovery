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
		// 目的表示
		shared_ptr<SimpleSprite> m_objectiveStr;
		// 消えるまでのタイマー
		TimeCounter m_invisibleTimer;

		bool m_isStart;
		bool m_isBeforeTimeUp = false;

		std::vector<std::function<void()>> m_timeUpEventFuncs;

	public:
		GameStartUI(const shared_ptr<Stage>& stage);

		void OnCreate()override;
		void OnUpdate()override;

		void Start()override;
		void Reset()override;
		bool IsGameBegin();

		void AddTimeUpEventFunc(const std::function<void()>& timeUpEventFunc) { m_timeUpEventFuncs.push_back(timeUpEventFunc); }
	};
}
