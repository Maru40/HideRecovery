#pragma once
#include "stdafx.h"
#include "Itabashi/OnlineManager.h"

namespace basecross {
	class MatchingSyncPlayerObject :public Online::OnlineComponent {
	public:
		MatchingSyncPlayerObject(const shared_ptr<GameObject>& owner);

		void OnCreate()override;

		// 部屋を作成
		void OnCreateRoom()override;
		// 部屋に参加
		void OnJoinRoom()override;
	};
}
