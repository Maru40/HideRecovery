
/*!
@file HideItem.cpp
@brief HideItemクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "HideItem.h"

namespace basecross {

	HideItem::HideItem(const std::shared_ptr<GameObject>& objPtr):
		ItemBase(objPtr)
	{}

	void HideItem::OnCreate() {
				
	}

}