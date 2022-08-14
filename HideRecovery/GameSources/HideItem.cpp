
/*!
@file HideItem.cpp
@brief HideItemクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "HideItem.h"
#include "Itabashi/ObjectHider.h"
#include "Itabashi/Item.h"

namespace basecross {

	HideItem::HideItem(const std::shared_ptr<GameObject>& objPtr):
		ItemBase(objPtr)
	{
	}

	void HideItem::OnCreate()
	{
		auto gameObject = GetGameObject();

		m_hider = gameObject->GetComponent<Operator::ObjectHider>();
		m_item = gameObject->GetComponent<Item>();
	}

}