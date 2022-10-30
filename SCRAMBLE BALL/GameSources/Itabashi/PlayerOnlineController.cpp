#include "stdafx.h"
#include "PlayerOnlineController.h"
#include "Patch/PlayerInputer.h"
#include "Watanabe/Component/PlayerStatus.h"
#include "OnlinePlayerSynchronizer.h"

template<class T>
T ConvertByteData(const std::uint8_t* bytes)
{
	return *(T*)bytes;
}

namespace basecross
{
	PlayerOnlineController::PlayerOnlineController(const std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{

	}

	void PlayerOnlineController::OnLateStart()
	{
		m_playerStatus = GetGameObject()->GetComponent<PlayerStatus>();
		m_onlinePlayerSynchronizer = GetGameObject()->GetComponent<OnlinePlayerSynchronizer>(false);
	}

	void PlayerOnlineController::OnUpdate()
	{
		auto playerStatus = m_playerStatus.lock();

		if (playerStatus->IsDead())
		{
			return;
		}

		auto onlinePlayerSynchronizer = m_onlinePlayerSynchronizer.lock();

		if (!onlinePlayerSynchronizer)
		{
			return;
		}

		if (PlayerInputer::GetInstance()->IsDesitionDown())
		{
			onlinePlayerSynchronizer->Teleport();
		}

		onlinePlayerSynchronizer->Move(PlayerInputer::GetMoveDirection());

		if (PlayerInputer::GetInstance()->IsDesitionDown())
		{
			onlinePlayerSynchronizer->OpenHidePlace();
		}

		if (PlayerInputer::GetInstance()->IsShot())
		{
			onlinePlayerSynchronizer->Shot();
		}

		onlinePlayerSynchronizer->Aim(PlayerInputer::GetInstance()->IsAim());
	}
}