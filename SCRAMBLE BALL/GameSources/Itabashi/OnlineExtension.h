#pragma once
#include "OnlineManager.h"

namespace basecross
{
namespace Online
{
	class OnlineExtension
	{
	public:
		static bool IsGameMaster() { return !OnlineManager::IsConnected() || OnlineManager::GetLocalPlayer().getIsMasterClient(); }
	};
}
}