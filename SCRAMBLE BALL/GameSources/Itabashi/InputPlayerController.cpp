#include "stdafx.h"
#include "InputPlayerController.h"
#include "Patch/PlayerInputer.h"
#include "Watanabe/Component/PlayerStatus.h"
#include "OnlinePlayerSynchronizer.h"
#include "Itabashi/Item.h"
#include "Maruyama/Player/Component/Teleport.h"
#include "Itabashi/PlayerControlManager.h"

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

		m_camera = GetStage()->GetView()->GetTargetCamera();
		m_playerControlManager = owner->GetComponent<PlayerControlManager>();
	}

	void InputPlayerController::OnUpdate()
	{
		if (!Online::OnlineManager::IsConnected())
		{
			m_playerControlManager.lock()->StateReset();
			return;
		}

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

		auto camera = m_camera.lock();

		if (camera)
		{
			onlinePlayerSynchronizer->ChangeDefaultForward(camera->GetForward());
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

	void InputPlayerController::OnCollisionEnter(std::shared_ptr<GameObject>& other)
	{
		auto playerStatus = m_playerStatus.lock();

		if (playerStatus->IsDead())
		{
			return;
		}

		auto item = other->GetComponent<Item>(false);

		if (!item)
		{
			return;
		}

		auto onlinePlayerSynchronizer = m_onlinePlayerSynchronizer.lock();

		if (!onlinePlayerSynchronizer)
		{
			return;
		}

		onlinePlayerSynchronizer->Aquisition(item);
	}
}