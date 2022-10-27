﻿#include "stdafx.h"
#include "Item.h"

namespace basecross
{
	Item::Item(const std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{

	}

	void Item::SetItemOwner(const std::shared_ptr<GameObject>& itemOwner, bool isOwnerParent)
	{
		m_itemOwner = itemOwner;
		auto gameObject = GetGameObject();

		if (isOwnerParent)
		{
			gameObject->SetParent(itemOwner);
		}
	}

	void Item::ReleaseItemOwner(bool isParentClear)
	{
		m_itemOwner.reset();

		auto gameObject = GetGameObject();

		if (isParentClear)
		{
			gameObject->SetParent(nullptr);
		}
	}
}