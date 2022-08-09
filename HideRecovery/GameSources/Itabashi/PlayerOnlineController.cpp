#include "stdafx.h"
#include "PlayerOnlineController.h"

namespace basecross
{
namespace Online
{
	std::wstring PlayerOnlineController::m_applicationID;

	PlayerOnlineController::PlayerOnlineController(const std::shared_ptr<GameObject>& owner) :
		Component(owner),
		m_client(*this, m_applicationID.c_str(), L"1.0")
	{

	}

	void PlayerOnlineController::OnUpdate()
	{

	}
}
}