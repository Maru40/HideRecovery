#include "stdafx.h"
#include "MatchingSyncPlayerObject.h"

namespace basecross {
	MatchingSyncPlayerObject::MatchingSyncPlayerObject(const shared_ptr<GameObject>& owner)
		:Online::OnlineComponent(owner)
	{}

	void MatchingSyncPlayerObject::OnCreate() {
	}

	void MatchingSyncPlayerObject::OnCreateRoom() {
	}
	void MatchingSyncPlayerObject::OnJoinRoom() {
	}
}