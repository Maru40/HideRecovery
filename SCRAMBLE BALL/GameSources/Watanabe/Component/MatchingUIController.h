#pragma once
#include "stdafx.h"
#include "Itabashi/OnlineManager.h"
#include "../UI/UIObjects.h"

namespace basecross {
	class MatchingUIController :public Online::OnlineComponent {
		shared_ptr<UIObjectCSVBuilder> m_builder;
	public:
		MatchingUIController(const shared_ptr<GameObject>& owner,
			const shared_ptr<UIObjectCSVBuilder>& builder);

		void OnCreate();

		// 部屋を作成
		void OnCreateRoom()override;
		// 部屋に参加
		void OnJoinRoom()override;
	};
}
