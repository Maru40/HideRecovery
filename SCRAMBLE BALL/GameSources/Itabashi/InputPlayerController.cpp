#include "stdafx.h"
#include "InputPlayerController.h"
#include "Patch/PlayerInputer.h"
#include "Watanabe/Component/PlayerStatus.h"
#include "OnlinePlayerSynchronizer.h"

#include "Maruyama/Player/Component/Teleport.h"

template<class T>
T ConvertByteData(const std::uint8_t* bytes)
{
	return *(T*)bytes;
}

namespace basecross
{
	InputPlayerController::InputPlayerController(const std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{

	}

	void InputPlayerController::OnLateStart()
	{
		auto owner = GetGameObject();
		m_playerStatus = owner->GetComponent<PlayerStatus>();
		m_onlinePlayerSynchronizer = owner->GetComponent<OnlinePlayerSynchronizer>(false);
		m_teleport = owner->GetComponent<Teleport>();
	}

	void InputPlayerController::OnUpdate()
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

		if (PlayerInputer::GetInstance()->IsOpenMap())
		{
			auto teleport = m_teleport.lock();
			teleport->OpenMap();
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