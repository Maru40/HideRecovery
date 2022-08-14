#include "stdafx.h"
#include "Item.h"

namespace basecross
{
	Item::Item(const std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{

	}

	void Item::SetItemOwner(const std::shared_ptr<GameObject>& itemOwner, bool isOwnerParent)
	{
		auto gameObject = GetGameObject();

		if (isOwnerParent)
		{
			gameObject->SetParent(itemOwner);
		}
	}

	void Item::ReleaseItemOwner(bool isParentClear)
	{
		auto gameObject = GetGameObject();

		if (isParentClear)
		{
			gameObject->SetParent(nullptr);
		}
	}

	std::shared_ptr<Item> Item::StageFindToItemId(const std::shared_ptr<Stage>& stage, int itemId)
	{
		if (!stage)
		{
			return nullptr;
		}

		for (auto& gameObject : stage->GetGameObjectVec())
		{
			auto item = gameObject->GetComponent<Item>(false);

			if (item && item->GetItemId() == itemId)
			{
				return item;
			}
		}

		return nullptr;
	}
}