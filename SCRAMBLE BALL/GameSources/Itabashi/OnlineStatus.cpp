#include "stdafx.h"
#include "OnlineStatus.h"

namespace basecross
{
namespace Online
{
	UniqueIdCreater<std::uint32_t> OnlineStatus::m_instanceIdCreater;
	std::unordered_map<std::uint32_t, std::weak_ptr<OnlineStatus>> OnlineStatus::m_onlineStatusMap;

	OnlineStatus::OnlineStatus(const std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{
		m_instanceIdCreater.SetMinId(1);
	}

	void OnlineStatus::SetCreateInstanceId()
	{
		if (m_instanceId != INVALID_INSTANCE_ID)
		{
			return;
		}

		m_instanceId = m_instanceIdCreater.CreateId();
		m_onlineStatusMap.insert(std::make_pair(m_instanceId, GetThis<OnlineStatus>()));
	}

	bool OnlineStatus::ChangeInstanceId(std::uint32_t instanceId)
	{
		if (!m_instanceIdCreater.ChangeId(m_instanceId, instanceId))
		{
			return false;
		}

		m_onlineStatusMap.insert(std::make_pair(instanceId, GetThis<OnlineStatus>()));
		m_onlineStatusMap.erase(m_instanceId);

		m_instanceId = instanceId;

		return true;
	}

	void OnlineStatus::OnDestroy()
	{
		if (m_instanceId == INVALID_INSTANCE_ID)
		{
			return;
		}

		m_onlineStatusMap.erase(m_instanceId);
	}

	std::shared_ptr<OnlineStatus> OnlineStatus::FindOnlineStatus(std::uint32_t instanceId)
	{
		auto find = m_onlineStatusMap.find(instanceId);

		return (find != m_onlineStatusMap.end()) ? find->second.lock() : nullptr;
	}

	std::shared_ptr<GameObject> OnlineStatus::FindOnlineGameObject(std::uint32_t instanceId)
	{
		auto onlineStatus = FindOnlineStatus(instanceId);
		return onlineStatus ? onlineStatus->GetGameObject() : nullptr;
	}
}
}