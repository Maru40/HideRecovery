#include "stdafx.h"
#include "HasBallEventExecuter.h"

namespace basecross {
	HasBallEventExecuter::HasBallEventExecuter(const shared_ptr<GameObject>& owner,
		const shared_ptr<ItemBag>& itemBag)
		:Component(owner), m_itemBag(itemBag)
	{}

	void HasBallEventExecuter::OnCreate() {
		m_playerObject = dynamic_pointer_cast<PlayerObject>(GetGameObject());
	}

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
				eventFunc(m_playerObject.lock());
			}
		}

		executionFlag = true;
	}

	void HasBallEventExecuter::AddEvent(const Func& haveBallEvent, const Func& notHaveBallEvent) {
		AddHaveBallEvent(haveBallEvent);
		AddNotHaveBallEvent(notHaveBallEvent);
	}

	void HasBallEventExecuter::AddHaveBallEvent(const Func& haveBallEvent) {
		m_haveBallEventList.push_back(haveBallEvent);
	}
	void HasBallEventExecuter::AddNotHaveBallEvent(const Func& notHaveBallEvent) {
		m_notHaveBallEventList.push_back(notHaveBallEvent);
	}
}