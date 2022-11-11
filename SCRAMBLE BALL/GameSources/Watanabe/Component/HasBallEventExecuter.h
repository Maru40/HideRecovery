﻿#pragma once
#include "stdafx.h"
#include "Maruyama/Player/Component/ItemBag.h"

namespace basecross {
	class HasBallEventExecuter :public Component {
		using Func = function<void(const shared_ptr<GameObject>&)>;

		// ボールを持っていないときのイベントリスト
		vector<Func> m_notHaveBallEventList;
		// ボールを持っているときのイベントリスト
		vector<Func> m_haveBallEventList;

		bool m_isExecutedNotHaveBallEvent = false;
		bool m_isExecutedHaveBallEvent = false;

		weak_ptr<ItemBag> m_itemBag;

		void ExecuteEvent(vector<Func>& eventList, bool& executionFlag);
	public:
		HasBallEventExecuter(const shared_ptr<GameObject>& owner,
			const shared_ptr<ItemBag>& itemBag);

		void OnCreate()override;
		void OnUpdate()override;

		void SetEvent(const Func& haveBallEvent, const Func& notHaveBallEvent);

		void SetHaveBallEvent(const Func& haveBallEvent);
		void SetNotHaveBallEvent(const Func& notHaveBallEvent);
	};
}
