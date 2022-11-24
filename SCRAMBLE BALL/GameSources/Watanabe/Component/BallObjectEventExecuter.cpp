#include "stdafx.h"
#include "BallObjectEventExecuter.h"

namespace basecross {
	BallObjectEventExecuter::BallObjectEventExecuter(const shared_ptr<GameObject>& owner)
		:Component(owner)
	{}

	void BallObjectEventExecuter::OnCreate() {
		OnEnable();
	}

	void BallObjectEventExecuter::OnDestroy() {
		OnDisable();
	}

	void BallObjectEventExecuter::OnEnable() {
		ExecuteEvent(m_enableEventList);
	}

	void BallObjectEventExecuter::OnDisable() {
		ExecuteEvent(m_disableEventList);
	}

	void BallObjectEventExecuter::ExecuteEvent(vector<Func>& eventList) {
		for (auto& func : eventList) {
			if (func)
				func();
		}
	}

	void BallObjectEventExecuter::AddEvent(const Func& enableEvent, const Func& disableEvent) {
		AddEnableEvent(enableEvent);
		AddDisableEvent(disableEvent);
	}

	void BallObjectEventExecuter::AddEnableEvent(const Func& enableEvent) {
		m_enableEventList.push_back(enableEvent);
	}
	void BallObjectEventExecuter::AddDisableEvent(const Func& disableEvent) {
		m_disableEventList.push_back(disableEvent);
	}
}