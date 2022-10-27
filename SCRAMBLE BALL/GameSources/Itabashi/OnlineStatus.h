#pragma once
#include <unordered_map>
#include "Utility/UniqueIdCreater.h"

namespace basecross
{
namespace Online
{
	class OnlineStatus : public Component
	{
	public:

		static constexpr std::uint32_t INVALID_INSTANCE_ID = 0;

	private:
		static UniqueIdCreater<std::uint32_t> m_instanceIdCreater;
		static std::unordered_map<std::uint32_t, std::weak_ptr<OnlineStatus>> m_onlineStatusMap;
		std::uint32_t m_instanceId = INVALID_INSTANCE_ID;

	public:

		OnlineStatus(const std::shared_ptr<GameObject>& owner);

		void SetCreateInstanceId();

		bool ChangeInstanceId(std::uint32_t instanceId);
		std::uint32_t GetInstanceId() const { return m_instanceId; }

		void OnUpdate() override {}

		void OnDestroy() override;

		void OnDraw() override {}

		static std::shared_ptr<OnlineStatus> FindOnlineStatus(std::uint32_t instanceId);

		static std::shared_ptr<GameObject> FindOnlineGameObject(std::uint32_t instanceId);
	};
}
}