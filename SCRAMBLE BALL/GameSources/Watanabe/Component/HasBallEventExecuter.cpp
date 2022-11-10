#include "stdafx.h"
#include "HasBallEventExecuter.h"

namespace basecross {
	HasBallEventExecuter::HasBallEventExecuter(const shared_ptr<GameObject>& owner,
		const shared_ptr<ItemBag>& itemBag)
		:Component(owner), m_itemBag(itemBag)
	{}

	void HasBallEventExecuter::OnCreate() {
	}
	void HasBallEventExecuter::OnUpdate() {
		// ボールを持っている
		if (m_itemBag.lock()->GetHideItem()) {
			for (auto& eventData : m_eventList) {
				if (eventData.func && !eventData.isExecuted) {
					eventData.func(GetGameObject());
					eventData.isExecuted = true;
				}
			}
		}
		else {
			for (auto& eventData : m_eventList) {
				if (eventData.isExecuted)
					eventData.isExecuted = false;
			}
		}
	}

	void HasBallEventExecuter::SetEvent(const Func& func) {
		m_eventList.push_back(EventData(func));
	}
}