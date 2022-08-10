#include "stdafx.h"
#include "OnlineTransformSynchronization.h"

namespace basecross
{
namespace Online
{
	struct OnlineTransformData
	{
		Vec3 position;
		Quat rotation;
	};

	OnlineTransformSynchronization::OnlineTransformSynchronization(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner)
	{
		
	}

	void OnlineTransformSynchronization::OnCreate()
	{
		m_transform = GetGameObject()->GetComponent<Transform>();
	}

	void OnlineTransformSynchronization::OnUpdate2()
	{
		auto transform = m_transform.lock();
		int localNumber = OnlineManager::GetLocalPlayer().getNumber();

		if (!transform || m_playerNumber == 0 || m_playerNumber != localNumber)
		{
			return;
		}

		OnlineTransformData data;
		data.position = transform->GetWorldPosition();
		data.rotation = transform->GetWorldQuaternion();

		OnlineManager::RaiseEvent(false, (std::uint8_t*)&data, sizeof(OnlineTransformData), EVENT_CODE);
	}

	void OnlineTransformSynchronization::OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes)
	{
		auto transform = m_transform.lock();

		if (!transform || m_playerNumber != playerNumber)
		{
			return;
		}

		OnlineTransformData data = *(OnlineTransformData*)bytes;
		
		transform->SetWorldPosition(data.position);
		transform->SetWorldQuaternion(data.rotation);
	}

}
}