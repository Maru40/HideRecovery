#include "stdafx.h"
#include "OnlineController.h"

namespace basecross
{
namespace Online
{
	std::wstring OnlineController::m_applicationID;

	OnlineController::OnlineController(const std::shared_ptr<GameObject>& owner) :
		Component(owner),
		m_client(*this, m_applicationID.c_str(), L"1.0")
	{

	}

	void OnlineController::OnUpdate()
	{

	}
}
}