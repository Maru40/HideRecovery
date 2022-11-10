#pragma once
#include "stdafx.h"
#include "Maruyama/Player/Component/ItemBag.h"

namespace basecross {
	class HasBallEventExecuter :public Component {
		using Func = function<void(const shared_ptr<GameObject>&)>;
		struct EventData {
			// 実行するイベント
			Func func;
			// 実行済みか
			bool isExecuted;

			EventData(const Func& func)
				:func(func), isExecuted(false) {}
		};
		vector<EventData> m_eventList;
		weak_ptr<ItemBag> m_itemBag;
	public:
		HasBallEventExecuter(const shared_ptr<GameObject>& owner,
			const shared_ptr<ItemBag>& itemBag);

		void OnCreate()override;
		void OnUpdate()override;

		void SetEvent(const Func& func);
	};
}
