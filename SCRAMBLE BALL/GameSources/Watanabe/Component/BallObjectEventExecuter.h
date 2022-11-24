#pragma once
#include "stdafx.h"

namespace basecross {
	class BallObjectEventExecuter :public Component {
		using Func = function<void()>;

		vector<Func> m_disableEventList;
		vector<Func> m_enableEventList;

		void ExecuteEvent(vector<Func>& eventList);
	public:
		BallObjectEventExecuter(const shared_ptr<GameObject>& owner);

		void OnEnable()override;
		void OnDisable()override;

		void OnCreate()override;
		void OnDestroy()override;

		void AddEvent(const Func& enableEvent, const Func& disableEvent);

		void AddEnableEvent(const Func& enableEvent);
		void AddDisableEvent(const Func& disableEvent);
	};
}
