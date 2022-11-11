#include "stdafx.h"
#include "HasBallEventExecuter.h"

namespace basecross {
	HasBallEventExecuter::HasBallEventExecuter(const shared_ptr<GameObject>& owner,
		const shared_ptr<ItemBag>& itemBag)
		:Component(owner), m_itemBag(itemBag)
	{}

	void HasBallEventExecuter::OnCreate() {}

	void HasBallEventExecuter::OnUpdate() {
		// ボールを持っている
		if (m_itemBag.lock()->GetHideItem()) {
			ExecuteEvent(m_haveBallEventList, m_isExecutedHaveBallEvent);
			m_isExecutedNotHaveBallEvent = false;
		}
		else {
			ExecuteEvent(m_notHaveBallEventList, m_isExecutedNotHaveBallEvent);
			m_isExecutedHaveBallEvent = false;
		}
	}

	void HasBallEventExecuter::ExecuteEvent(vector<Func>& eventList, bool& executionFlag) {
		if (executionFlag)
			return;

		for (auto& eventFunc : eventList) {
			// 関数が有効
			if (eventFunc) {
				eventFunc(GetGameObject());
			}
		}

		executionFlag = true;
	}

	void HasBallEventExecuter::SetEvent(const Func& haveBallEvent, const Func& notHaveBallEvent) {
		SetHaveBallEvent(haveBallEvent);
		SetNotHaveBallEvent(notHaveBallEvent);
	}

	void HasBallEventExecuter::SetHaveBallEvent(const Func& haveBallEvent) {
		m_haveBallEventList.push_back(haveBallEvent);
	}
	void HasBallEventExecuter::SetNotHaveBallEvent(const Func& notHaveBallEvent) {
		m_notHaveBallEventList.push_back(notHaveBallEvent);
	}
}