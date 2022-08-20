#pragma once
#include "stdafx.h"
#include "OnlineManager.h"

namespace basecross
{
	class MatchStageRoomer : public Online::OnlineComponent
	{
	public:
		MatchStageRoomer(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;

		void OnConnected() override;

		void OnCreateRoom() override;
		void OnJoinRoom() override;

		void OnCreateRoomFailed(int errorCode) override;
	};
}