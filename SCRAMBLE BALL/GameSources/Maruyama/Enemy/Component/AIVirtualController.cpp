
/*!
@file AIVirtualController.cpp
@brief AIVirtualControllerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "AIVirtualController.h"

#include "Watanabe/Component/PlayerStatus.h"
#include "Itabashi/OnlinePlayerSynchronizer.h"

#include "Itabashi/Item.h"

namespace basecross {

	AIVirtualController::AIVirtualController(const std::shared_ptr<GameObject>& objPtr):
		Component(objPtr),
		m_inputDirection(Vec2(0.0f))
	{}

	void AIVirtualController::OnStart() {
		auto object = GetGameObject();

		m_status = object->GetComponent<PlayerStatus>(false);
		m_onlineSynchronizer = object->GetComponent<OnlinePlayerSynchronizer>(false);
	}

	void AIVirtualController::SetInputDirection(const Vec2& direction) {
		m_inputDirection = direction;
	}

	void AIVirtualController::OnCollisionEnter(std::shared_ptr<GameObject>& other) {
		auto playerStatus = m_status.lock();

		if (playerStatus->IsDead())
		{
			return;
		}

		auto item = other->GetComponent<Item>(false);

		if (!item)
		{
			return;
		}

		auto onlinePlayerSynchronizer = m_onlineSynchronizer.lock();

		if (!onlinePlayerSynchronizer)
		{
			return;
		}

		onlinePlayerSynchronizer->Aquisition(item);
	}

}