#include "stdafx.h"
#include "OnlineTransformSynchronization.h"

namespace basecross
{
namespace Online
{
	bool OnlineTransformSynchronization::OnlineTransformData::operator==(const OnlineTransformData& other) const
	{
		return position == other.position && rotation == other.rotation;
	}

	OnlineTransformSynchronization::OnlineTransformSynchronization(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner)
	{
		
	}

	OnlineTransformSynchronization::OnlineTransformData OnlineTransformSynchronization::GetTransformData() const
	{
		OnlineTransformData data;

		auto transform = m_transform.lock();

		data.position = transform->GetWorldPosition();
		data.rotation = transform->GetWorldQuaternion();

		return data;
	}

	void OnlineTransformSynchronization::OnCreate()
	{
		m_transform = GetGameObject()->GetComponent<Transform>();
	}

	void OnlineTransformSynchronization::OnUpdate2()
	{
		auto transform = m_transform.lock();
		int localNumber = OnlineManager::GetLocalPlayer().getNumber();

		if (!transform || m_onlinePlayerNumber == OnlineManager::INVALID_ONLINE_PLAYER_NUMBER || m_onlinePlayerNumber != localNumber)
		{
			return;
		}

		auto data = GetTransformData();

		if (data == m_beforeData)
		{
			return;
		}

		OnlineManager::RaiseEvent(false, (std::uint8_t*)&data, sizeof(OnlineTransformData), EVENT_CODE);

		m_beforeData = data;
	}

	void OnlineTransformSynchronization::OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes)
	{
		auto transform = m_transform.lock();

		if (!transform || m_onlinePlayerNumber != playerNumber || eventCode != EVENT_CODE)
		{
			return;
		}

		OnlineTransformData data = *(OnlineTransformData*)bytes;
		
		transform->SetWorldPosition(data.position);
		transform->SetWorldQuaternion(data.rotation);
	}

}
}