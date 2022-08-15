/*!
@file ItemBag.cpp
@brief ItemBagクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "ItemBag.h"

#include "ItemBase.h"
#include "HideItem.h"

#include "MaruUtility.h"

#include "HideItemObject.h"
#include "Itabashi/Item.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// パラメータ
	//--------------------------------------------------------------------------------------

	ItemBag_Parametor::ItemBag_Parametor() :
		ItemBag_Parametor(0, 30)
	{}

	ItemBag_Parametor::ItemBag_Parametor(const int numStartOwn, const int numMaxOwn) :
		numStartOwn(numStartOwn), numMaxOwn(numMaxOwn)
	{}

	//--------------------------------------------------------------------------------------
	/// アイテムバッグ本体
	//--------------------------------------------------------------------------------------

	ItemBag::ItemBag(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr), m_param(Parametor())
	{}

	void ItemBag::OnCreate() {

		for (int i = 0; i < m_param.numStartOwn; i++) {
			auto object = GetStage()->AddGameObject<HideItemObject>();
			auto item = object->GetComponent<Item>();
			AddItem(item);
		}
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	void ItemBag::AddItem(const std::shared_ptr<Item>& item)
	{
		m_items.push_back(item);
		item->SetItemOwner(GetGameObject(), false);
	}

	void ItemBag::RemoveItem(const std::shared_ptr<Item>& item)
	{
		auto find = std::find_if(m_items.begin(), m_items.end(), [&item](const std::weak_ptr<Item>& itemWeak) {return itemWeak.lock() == item; });

		if (find != m_items.end())
		{
			find->lock()->ReleaseItemOwner(false);

			m_items.erase(find);
		}
	}

	bool ItemBag::IsAcquisition(const std::shared_ptr<Item>& item) {
		if (m_items.size() < m_param.numMaxOwn) { //バッグの空きがあったら
			return true;
		}

		return false;
	}

	std::shared_ptr<HideItem> ItemBag::GetHideItem() const {
		for (auto& itemWeak : m_items) {

			auto item = itemWeak.lock();

			if (!item)
			{
				continue;
			}

			if (auto hideItem = item->GetGameObject()->GetComponent<HideItem>(false)) {
				return hideItem;
			}
		}

		return nullptr;
	}
}