
/*!
@file ItemBag.cpp
@brief ItemBag�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "ItemBag.h"

#include "ItemBase.h"
#include "HideItem.h"

#include "MaruUtility.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �p�����[�^
	//--------------------------------------------------------------------------------------

	ItemBag_Parametor::ItemBag_Parametor() :
		ItemBag_Parametor(3)
	{}

	ItemBag_Parametor::ItemBag_Parametor(const uint32 numMaxOwn) :
		numMaxOwn(numMaxOwn)
	{}

	//--------------------------------------------------------------------------------------
	/// �A�C�e���o�b�O�{��
	//--------------------------------------------------------------------------------------

	ItemBag::ItemBag(const std::shared_ptr<GameObject>& objPtr):
		Component(objPtr), m_param(Parametor())
	{}

	void ItemBag::OnCreate() {

	}

	//--------------------------------------------------------------------------------------
	/// �A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	void ItemBag::AddItem(const std::shared_ptr<ItemBase>& item) {
		m_items.push_back(item);
	}

	void ItemBag::RemoveItem(const std::shared_ptr<ItemBase>& item) {
		ex_weak_ptr<ItemBase> exweakItem = item;
		maru::Utility::RemoveVec(m_items, exweakItem);
	}

	bool ItemBag::IsAcquisition(const std::shared_ptr<ItemBase>& item) {
		if (m_items.size() < m_param.numMaxOwn) { //�o�b�O�̋󂫂���������
			return true;
		}

		return false;
	}

	std::shared_ptr<HideItem> ItemBag::GetHideItem() const {
		for (auto& item : m_items) {
			if (auto hideItem = item->GetGameObject()->GetComponent<HideItem>(false)) {
				return hideItem;
			}
		}

		return nullptr;
	}
}