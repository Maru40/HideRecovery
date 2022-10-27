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

		auto hidePlace = maru::MyRandom::RandomArray(hidePlaces);
		auto objectHider = hideItem->GetGameObject()->GetComponent<Operator::ObjectHider>();

		objectHider->Hide(hidePlace->GetHidePosition());
		hidePlace->SetHideItem(hideItem);

		auto hideItemId = hideItem->GetGameObject()->GetComponent<Online::OnlineStatus>()->GetInstanceId();
		auto hidePlaceId = hidePlace->GetGameObject()->GetComponent<Online::OnlineStatus>()->GetInstanceId();

		Online::OnlineManager::RaiseEvent(false, (std::uint8_t*)&OnlineItemHideData(hideItemId,hidePlaceId), sizeof(OnlineItemHideData), ITEM_RANDOM_HIDE_EVENT_CODE);
	}

	void OnlineGameItemManager::ExecuteRandomHideItem(std::uint32_t hideItemInstanceId, std::uint32_t hidePlaceInstanceId)
	{
		auto hidePlace = Online::OnlineStatus::FindOnlineGameObject(hidePlaceInstanceId)->GetComponent<HidePlace>();
		auto hideItemObject = Online::OnlineStatus::FindOnlineGameObject(hideItemInstanceId);

		auto hideItem = hideItemObject->GetComponent<HideItem>();
		auto itemHider = hideItemObject->GetComponent<Operator::ObjectHider>();

		itemHider->Hide(hidePlace->GetHidePosition());
		hidePlace->SetHideItem(hideItem);
	}
}