#include "stdafx.h"
#include "OnlineGameItemManager.h"
#include "Maruyama/Item/HideItem.h"
#include "OnlineStatus.h"
#include "Maruyama/StageObject/HidePlace.h"
#include "ObjectHider.h"
#include "Maruyama/Utility/Random.h"
#include "Maruyama/Utility/Utility.h"

namespace basecross
{
	struct OnlineItemHideData
	{
		std::uint32_t hideItemInstanceId;
		std::uint32_t hidePlaceInstanceId;

		OnlineItemHideData(std::uint32_t hideItemInstanceId, std::uint32_t hidePlaceInstanceId) :
			hideItemInstanceId(hideItemInstanceId),
			hidePlaceInstanceId(hidePlaceInstanceId)
		{

		}
	};

	/// <summary>
	/// 配列の要素をランダムに取得する
	/// </summary>
	template <class Iterator>
	auto& GetRandom(Iterator first, Iterator last)
	{
		static std::random_device device;
		static std::mt19937_64 randomEngine(device());

		auto distance = std::distance(first, last);

		return *(first + std::uniform_int_distribution<decltype(distance)>(0, distance - 1)(randomEngine));
	}

	OnlineGameItemManager::OnlineGameItemManager(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner)
	{

	}

	void OnlineGameItemManager::OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes)
	{
		if (eventCode == ITEM_RANDOM_HIDE_EVENT_CODE)
		{
			auto data = *(OnlineItemHideData*)bytes;
			ExecuteRandomHideItem(data.hideItemInstanceId, data.hidePlaceInstanceId);
			return;
		}
	}

	void OnlineGameItemManager::RandomHideItem(const std::shared_ptr<HideItem>& hideItem)
	{
		auto hidePlaces = maru::Utility::FindComponents<HidePlace>();

		auto result = std::remove_if(hidePlaces.begin(), hidePlaces.end(),
			[](const std::shared_ptr<HidePlace>& hidePlace) {return !hidePlace->CanPutable(); });

		auto& hidePlace = GetRandom(hidePlaces.begin(), result);
		hidePlace->PutHideItem(hideItem);

		auto hideItemId = hideItem->GetGameObject()->GetComponent<Online::OnlineStatus>()->GetInstanceId();
		auto hidePlaceId = hidePlace->GetGameObject()->GetComponent<Online::OnlineStatus>()->GetInstanceId();

		Online::OnlineManager::RaiseEvent(true, (std::uint8_t*)&OnlineItemHideData(hideItemId,hidePlaceId), sizeof(OnlineItemHideData), ITEM_RANDOM_HIDE_EVENT_CODE);
	}

	void OnlineGameItemManager::ExecuteRandomHideItem(std::uint32_t hideItemInstanceId, std::uint32_t hidePlaceInstanceId)
	{
		auto hidePlace = Online::OnlineStatus::FindOnlineGameObject(hidePlaceInstanceId)->GetComponent<HidePlace>();
		auto hideItem = Online::OnlineStatus::FindOnlineGameObject(hideItemInstanceId)->GetComponent<HideItem>();

		hidePlace->PutHideItem(hideItem);
	}
}